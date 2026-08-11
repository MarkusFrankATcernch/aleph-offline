/*
 *
 * Revision 1.1.1.1  2006/06/14
 * Kernlib  utility for locf, locb on LP64 architectures  H. Vogt
 * (AMD64/Intel EM64T and IA64) 
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*>    ROUTINE LOCF
  CERN PROGLIB# N100    LOCF            .VERSION KERNFOR  4.36  930602
*/
/*>    ROUTINE LOCB
  CERN PROGLIB# N101    LOCB            .VERSION KERNFOR  4.36  930602
*/

unsigned int chkloc(iadr)
   char *iadr;
{
  /* 64 bit architectures may exceed the 32 bit address space !               */

  /* AMD64/Intel EM64T architectures have the dynamic segments above
     0x80000000000 and the stack immediately below this whereas the text and
     data segments are staring from 0x400000. The implementations address
     space is limited to 0x00007fffffffffff.
     Allocated memory with malloc/calloc is starting from the end of text and
     data segments upwards.                                                   

     IA64 architectures have the dynamic segments are above 0x2000000000000000,
     the stack is above 0x8000000000000000, the data segments starts at 
     0x6000000000000000 and the text segments start at 0x4000000000000000   
     Allocated memory with malloc/calloc is starting from the end of
     data segments upwards. All addresses here are expected to be in
     the data segment area.                                                   */

  /* K. McCarty: On Alpha, on the other hand, I couldn't find any docs for the
     Linux segmentation for virtual memory.  Judging by a test machine
     (escher.debian.org), the data section starts slightly above 0x120010000.
     On the other hand the stack grows downward from 0x120000000. */
  /* U. Schwickerath on aarch64 the stack grows downward as for Alpha */

   const unsigned long mask=0xffffffff00000000;
   static unsigned long limit=0x00000000ffffffff;
   unsigned long jadr=((unsigned long) iadr & mask);
#if defined(__linux__)
   /* In linux the common blocks and all other static data
      are located in the .bss data segment or in the heap.
      We offset the start address of each data segment found
      from the address given.
      The data segments are defined in /proc/self/maps
      and have no underlying file in sections like this:
      "
      55555555a000-55555559f000 rw-p 00000000 00:00 0
      55555559f000-5555555c0000 rw-p 00000000 00:00 0                          [heap]
      "

      M.Frank EP/LCB-CS
    */
   static uint64_t  bss_start = 0;
   static uint64_t  num_segments = 0;
   static uint64_t* segment_start = NULL;
   static uint64_t* segment_end   = NULL;

   if( 0 == bss_start )  {
     uint64_t max_segment = 1000;
     uint64_t mlen = max_segment*sizeof(uint64_t);
     ssize_t  len = 0;
     size_t   sl = 0;
     char*    text = 0;
     FILE*    in = fopen("/proc/self/maps", "r");

     segment_start = (uint64_t*)malloc(mlen);
     segment_end   = (uint64_t*)malloc(mlen);
     segment_start[0] = segment_end[0] = 0;
     for( len=0, sl=0; (len=getline(&text, &sl, in)) != -1; )  {
       if( strstr(text, "00:00 0") != 0 )  {
         uint64_t start=0, end=0;
         if( 2 != sscanf(text, "%lx-%lx ", &start, &end) )  {
           perror("Failed to get address of data segment.");
           exit(EINVAL);
         }
         segment_end[num_segments] = end;
         segment_start[num_segments] = start;
         ++num_segments;
         if( num_segments > max_segment )  {
           /// Should not really end up here (1000 is way enough)
           max_segment += 500;
           mlen         = max_segment*sizeof(uint64_t);
           segment_start = (uint64_t*)realloc(segment_start, mlen);
           segment_end   = (uint64_t*)realloc(segment_end, mlen);
         }
       }
     }
     if( 0 == bss_start ) bss_start = segment_start[0];
     close(in);
     if( text ) free(text);
     printf("+++ Found %ld data segments. "
            "LOCF checks if address is with data segments.\n",num_segments);
     fflush(stdout);
   }
   uint64_t check_addr = ((uint64_t)iadr);
   for( size_t i=0; i<num_segments; ++i )  {     
     if( check_addr >= segment_start[i] && check_addr < segment_end[i] )  {
       iadr -= segment_start[i];
       break;
     }
   }
   jadr  = ((unsigned long) iadr & mask);
   if ( jadr != 0) {
#elif defined (__ia64__)
    if ( jadr != 0x6000000000000000) {
#elif defined (__alpha__)
    if ( jadr != 0x0000000100000000) {
#elif  defined(__powerpc__)
    if ( jadr != 0x0000000100000000) {
      fprintf(stderr, "ppc64\n");
#elif  defined(__DARWIN__)
    if ( jadr != 0x0000000100000000) {
      fprintf(stderr, "darwin\n");
#else  /* amd64, aarch64, arm64 */
    if ( jadr != 0) {
      fprintf(stderr, "amd64, ppc64 or aarch64\n");
#endif
      fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      fprintf(stderr, "LOCB/LOCF: address %p exceeds the 32 bit address space\n", iadr);
      fprintf(stderr, "or is not in the data segments\n");
      fprintf(stderr, "This may result in program crash or incorrect results\n");
      fprintf(stderr, "Therefore we will stop here\n");
      fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      exit (999);
    }
    jadr=((unsigned long) iadr & limit);
    return ((unsigned) jadr);
}
/*> END <----------------------------------------------------------*/

