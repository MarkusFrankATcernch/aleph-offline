# ==========================================================================
#   Software for the ALEPH experiment
# --------------------------------------------------------------------------
#  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
#  All rights reserved.
# 
#  For the licensing terms see ALSOFTINSTALL/LICENSE.
#  For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
# 
#  Author     : M.Frank
# 
# ==========================================================================

__version__ = "1.0"
__author__  = "Markus Frank <Markus.Frank@cern.ch>"
__usage__   = f"""

python gen_header.py --bank FTFT --output . 
"""

all_banks = """\
ACUT ADBL ADBN ADBR ADBS AFID AJOB AKIN ALCB ALLR ALP1 ALPB ALPH ALRP
 APRO ARUN ASEV ASIM ATIT ATYP BATT BCAR BCCL BCGN BCHG BCNT BCON BCPD
 BCPO BCSC BCSL BCTR BD1B BD2B BD3B BDAD BDAT BDES BDFS BDRQ BENE BEXA
 BHAB BHIT BHZ0 BKGT BKK7 BKLI BKM7 BKMC BKMD BKMN BLAS BLCT BLPR BLQP
 BLUM BMAT BMCL BMEL BMIX BMMA BNAM BOFS BOMA BOMB BOMC BOMD BOME BOMG
 BOMM BOMO BOMP BOMQ BOMR BOSK BOSK BPBB BPCO BPER BPG1 BPHY BPLI BPOR
 BPOS BPRO BPTR BRAN BRDL BRLI BROT BRUC BS4B BSCL BSET BSLU BSPF BTAP
 BTHO BTME BTPN BVOL BXLI BXYZ BYEA CADB CALO CAPA CCT1 CEXT CHPR CHRL
 CHYP CLTC COBS COCR COG1 COL2 COPT CPAR CPRO CRFT CRL3 CSTC CTC2 CTYP
 D4CD DBTG DCRL DDLT DE14 DECO DEID DENF DENW DEVT DEWI DFMC DFOT DGAC
 DGAM DGID DGPC DHCO DHEA DHRL DJET DKNK DLJT DMJT DMLT DMSC DMUO DNEU
 DPOB DRES DSIC DTBP DTHR DTMC DTRA DVER DVMC DVRS DWES DYV0 E2SC E3SC
 E4DE E4SC E5SC EAGC EALF EALI EAUX EBOK EBOS EBPL ECCA ECDF ECGF ECGN
 ECHE ECIL ECLK ECLU ECMC ECMT ECNC ECNS ECOB ECOL ECOR ECRP ECRQ ECRS
 ECT1 ECTE ECTY EDDB EDGE EDKB EDNO EDPA EDST EDWI EECB EF4N EFAC EFAS
 EFCA EFET EFIP EFOL EFTD EFZC EGAZ EGCO EGID EGLO EGMA EGMD EGNC EGNE
 EGOZ EGPA EGPC EGPR EGRP EGST EGTE EGTH EGVP EHDS EHGF EHPA EHWI EHYP
 EIBP EIBR EIDT EIGA EJET EKLS EKPL EL1N EL2N ELNF ELOC ELTY ELZE EMAT
 EMIP EMIR EMOD EMSK ENNO EPAR EPCC EPHY EPLN EPSC EPTY EQNT EQTY EREG
 ERL3 EROR EROW ERRF ERRR ESCO ESDA ESEC ESHI ESLO ESLW ESPA ESRH ESRL
 ESSC ESTK ESTO ESTY ESWI ETC2 ETCK ETCO ETDI ETHT ETKC ETP1 ETSC ETTR
 ETYV EVEH EVLF EVLS EVOL EW1N EW2N EWDI EWGA EWHE EWHI EWHT EWKW EWRG
 EWTR EXRG EXRO EZTH FCON FGTP FICL FKAL FKIN FLCO FLTR FPOI FPOL FRFT
 FRID FRTL FSCO FSHO FSTR FTCL FTCM FTOC FTTM FVCL FVER FXTR FZFR GDEC
 H1EC H2EC H3EC H4EC HBAD HBAR HBCP HBDE HBGE HBMT HBPM HCAL HCCO HCCV
 HCHI HCIN HCLB HCLU HCNF HCOL HCOS HCSE HCTB HCTE HCTG HCTH HCUE HCVL
 HDRC HDTE HECP HEDE HEGE HEMT HEND HEPM HETC HGEA HHPS HIFC HIMC HIMM
 HINF HLAY HLOD HLTU HLWD HMAD HNGR HNTO HPB1 HPB2 HPCO HPDI HPDS HPEA
 HPEB HPHT HRDT HROA HSBA HSCA HSCE HSCI HSCO HSCW HSDA HSEC HSLO HSPE
 HSPH HSPT HSSR HSTD HSTO HSTP HSTR HT0C HT0W HTDI HTDO HTHT HTID HTPA
 HTRE HTTD HTTR HTUB HTUE HTUS HTXD HVBA HVEA HVEB HWDI HWHT HWNO HWTD
 HWTR HZEF IALI IASL IATT IAUT ICAE ICAL ICCO ICMR ICTL ICTR ICUT IDCR
 IDHR IDIG IDRP IDSP IDTC IEDD IEFF IET0 IEWP IEXC IGCL IGEO IGTL IHDQ
 IHIT IHOC ILIV ILYR IMAT IMIX IMMA IMPA INLI IPCO IPJT IPMO IPOS IQXT
 IRES IRFE IRJT IRRF ISCE ISCI ISCO ISEE ISFE ISLO ISSR ISTA ISUR ISWB
 ITCC ITCO ITDI ITDL ITDR ITFN ITFP ITFT ITHT ITKP ITMA ITME ITOR ITPC
 ITRM ITTR ITWP IVOL IWCR IWST IXBW IXCR IXRP IXSI IXST IXTR IZCA IZDB
 IZFE IZNL IZRS IZSC JBER JCON JEDS JEST JHDX JPAS JSUM JTDX JTRE KCAR
 KEVH KHIS KINE KJOB KLIN KLUN KMAR KORL KPAR KPOL KRAN KREF KRUN KSEC
 KSHO KVOL KW4F KWGT KWTK KXME KZFR LACC LALI LBAK LCAL LCCA LCEL LCLU
 LCMC LCPG LCRA LCRE LCSH LCWS LDRE LDST LDWP LECA LEHI LFIL LFIP LIDT
 LIFL LLAY LMAP LMTY LOBJ LOCL LOLE LONL LPAS LPDA LPFS LPFT LPLS LPLT
 LPMO LPZS LPZT LRWG LSCO LSDA LSHI LSHO LSIN LSLO LSNT LTDI LTHT LTRK
 LTTR LUMI LUPA LVHI LWDI LWHI LWHT LWRG LWTR LXAC LXBM LXCR LXDA LXER
 LXFC LXGB LXHA LXIP LXKG LXLY LXNM LXQS LXRF LXSP LXTR LXUN LXVA LXWG
 LYFT LYLT LYZT LZAT LZFT LZLT LZPT LZVT LZZT M1SG M1TG M2SG M3BG MBAG
 MBBG MBLG MBSG MBTG MCAD MCPE MCUT MEBG MECG MELG MESG METG MGSC MHIT
 MMAG MMBG MMLG MMSG MMTG MONE MSLO MTHR MUDG MUDI MUDT MUEX MUHT MUID
 MUNO MUOG NBIP NDAR NDBM NDCL NDDE NDEJ NDGC NDHE NDJT NDLV NDMS NDNT
 NDOB NDPH NDST NDTK NDV0 NEHE NEVE NRFT NVEC OALE OBUN OCOM OROP OSCO
 OSLO OSTS PART PASC PASL PATT PBCR PCAL PCHA PCHY PCOB PCOI PCPA PCQA
 PCRL PCST PDLT PEBR PECO PEHY PEID PEMH PEOB PEOT PEPT PESM PEST PETB
 PEWD PEWH PEWI PFER PFHR PFIX PFRF PFRT PFXT PGAC PGID PGPA PGPC PHCO
 PHER PHHY PHMA PHMH PHOB PHPR PHST PHTO PIDI PITM PKST PLID PLJT PLPD
 PLSC PLSD PMAT PMEL PMG1 PMIX PMLT PMMA PMOR PMSC PMSK PNEU PNFX PPDS
 PPHY PPOB PPOS PPRL PROT PRPW PRTM PSCO PSPO PT2X PTBC PTCO PTEX PTHR
 PTMA PTME PTML PTNC PTPX PTST PTUN PTYP PVOL PWEI PYCH PYER PYFR PYNE
 PYV0 QBDE QBSM QIPC QNDE QNSM QPDE QPSM QTDE QTSM QUG1 RALE REVH RFBM
 RFLY RFOM RFQU RFRB RFSA RFTR RFUN RFVC RFWT RHAH RHOH RKSE RL01 RLEP
 RLUM RNF2 RNL2 RNR2 RREP RS4B RSLU RTLO RUNE RUNH RUNR RXYZ SACO SADI
 SAHT SALG SALI SATR SATT SBHA SCAX SCBP SCCP SCFD SCGA SCHU SCLB SCLS
 SCLU SCOO SCOR SCPH SCRP SDCO SDPD SECA SECB SECT SFTH SFTR SGMN SGPA
 SHLD SHOT SIBX SIDI SIFO SIGO SIHI SIHT SIID SILH SILU SINF SINT SIOR
 SIPO SISC SITC SITR SIX2 SIXA SIXP SKAN SKCP SKPD SLAY SLUM SMAP SMAT
 SMEL SMIX SMMA SMPD SNOI SPAT SPCP SPDA SPED SPHD SPOS SRCO SREC SRKT
 SROT SRTD SSBP SSCO SSCP SSKP SSLO SSPP SSTP SSUP STCP STHI STME STRK
 STYP SUCP SUPA SVOL SWPA SWTU SZTH T0GL T0RL T1CL T1FC T1FT T1TL T2FC
 T2XS T3CC T3FC T3RR T4RR TAB2 TABS TADB TALI TANF TARC TARE TATC TBCO
 TBDR TC1X TC2X TC3X TC4X TC5X TC6X TC7X TCAL TCCN TCGD TCGX TCHA TCLB
 TCLU TCOL TCON TCPL TCPX TCRC TCRL TCSX TCTC TCUC TCUR TDBS TDEF TDFV
 TDOC TDPV TDVV TDXC TE1R TELS TERN TERR TEXB TEXS TFT0 TGCL TGFT TGMA
 TGTL TGTN THPL THRP TIND TINF TISL TKAP TLAA TLAN TLAS TLCE TLCP TLCT
 TLFC TLNK TLPA TLRL TMCL TMSH TMTL TMTY TNCO TNFC TNFV TNLO TNMR TNOG
 TNOJ TNOS TNRN TOER TP1X TP3X TP4X TP5X TP6X TPAD TPAZ TPCH TPCO TPDI
 TPDX TPHE TPHT TPHV TPLS TPMO TPOS TPOT TPRF TPRL TPRO TPRZ TPSM TPTE
 TPUL TPWZ TPXS TRCL TRDI TREP TREX TRIK TRIR TRLE TRSC TRZS TSCE TSCI
 TSCL TSCO TSDI TSEE TSGM TSHP TSIM TSIR TSLE TSLO TSOR TSPU TSRL TSSR
 TSTC TSWP TTCC TTHE TTHT TTRC TVVD TVXY TWAT TWCO TWDI TWHT TWIR TWIT
 TWNF TWOL TWPU TWRC TWRD TWRE TWRF TWRR TWTA TWTB TWTC TWZA TWZZ TZCV
 UFG2 VAGB VALC VATT VBPC VBWP VCAB VCHL VCLU VCOD VCOM VCPC VCPL VCSG
 VCTL VCUT VCZC VDAL VDCC VDCE VDCM VDCO VDDT VDEL VDEM VDEP VDFK VDGC
 VDHT VDIZ VDLA VDLH VDME VDMR VDMS VDPR VDRL VDRO VDSI VDSL VDSM VDSP
 VDSS VDTD VDTE VDXY VDZT VELE VERT VEVH VFEB VFHL VFLG VFMC VFPH VGAN
 VGCL VGGW VGHC VGLB VGMD VGPA VGTL VGXC VHBV VHIV VHLS VHMP VHOT VHPP
 VHPV VHYB VKIL VLGB VLST VM5C VMAT VMBE VMBU VMEL VMGN VMIX VMMA VMPC
 VMRE VMTC VMUC VMUN VMWC VNGB VNLC VNPC VNZC VOCM VOFL VOLU VONS VOPD
 VORD VORP VOVC VPAR VPCH VPCO VPEC VPES VPHN VPHO VPLH VPOS VRCN VRDO
 VREG VREJ VRGC VRLC VRLD VROT VRRR VSBD VSLT VSPL VSWP VTCE VTCK VTER
 VTMA VTME VTPA VTRA VTRP VTRS VTSC VTSK VTUC VTWC VTXT VUEC VUFK VVOL
 VWC1 VWC2 VWGE VWGM VWRL VWS1 VWS2 VXCH VZEW VZMR VZPW WIDE WIDN X1AD
 X1CA X1CB X1DA X1DI X1ER X1HI X1IP X1RG X1SC X1SM X1SV X1TD X1TH X1TT
 X1TV X2CO X2DF X2LO X2MS X2TB X31X X3EC X3EW X3HC X3IT X3L2 X3LU X3PS
 X3TM X3TO X3TP X3X3 XCNT XHVB XSGE XSHI XSSC XTBN XTCN XTDI XTEB XTMS
 XTOP XTRB XTTL YCFT YCMA YCPE YCTR YKCI YKNK YKSP YLV0 YNFT YNLI YNMA
 YNPE YNTR YNVH YS0L YSBO YSCL YSCT YSFT YSMO YSTL YSTV YSVT YSVX YTCP
 YTGP YTPP YTRL YTVP YV0V YV1C YVNC YVXL ZPFR"""

import os
import sys
import pdb

lbf_file = ''
bend = ' ========================================================='
header_match = '..................................................'
global_debug = False


# ==========================================================================
def debug(msg):
  if global_debug:
    print(msg)

def to_string_list(comments, fmt='%s'):
  s = ''
  for c in comments:
    while len(c)>1 and c[len(c)-1] == ' ':
      c = c[:-1]
    s = s + fmt%(c,)+'\n'
  return s[:-1]

# ==========================================================================
#
#
#
#  Author     : M.Frank
# 
# ==========================================================================
class header_generator:
  # ========================================================================
  def __init__(self):
    self.header_data = ''
    self.have_stringifiers = False

  # ========================================================================
  def output(self, text):
    self.header_data = self.header_data + text + '\n'

  # ========================================================================
  def gen_header(self, bnam, comments, subheader, variables):
    """
         Generate header from ALEPH bank inventory
         \version 1.0
         \author  M.Frank
    """
    bracket_open = '{'
    bracket_close = '}'
    lower_bnam = bnam.lower()
    upper_bnam = bnam.upper()

    num_words_bank = 0
    num_words_per_bank = -1
    bank_is_object_table = False
    match1 = 'number of words'
    match2 = '# of words'
    if len(subheader):
      for c in subheader:
        idx1 = c.lower().find(match1)
        idx2 = c.lower().find(match2)
        if idx1 > 0 or idx2 > 0:
          bank_is_object_table = True
          num_words_per_bank = 0
          idx3 = c.find('(')
          if idx3 > 0:
            it = c.split('(')[1].split(')')[0][1:]
            num_words_per_bank = int(it)

    header_stringify_decl = f"""/// Stringified data header of bank type "{upper_bnam}"
      std::string to_string_header()  const;"""
    data_stringify_decl = f"""/// Stringified object data of single bank of type "{upper_bnam}"
      std::string to_string_data()  const;"""
    object_stringify_decl = f"""      /// Stringified object data of bank type "{upper_bnam}"
      std::string to_string()  const;"""

    var_comments = []
    for v in variables:
      var_comments.append( f'{to_string_list(v, '    *  %s')}' )

    table_stringify_decl = ''
    if bank_is_object_table:
      table_stringify_decl = f"""\
      /// Stringified object table of bank type "{upper_bnam}"
      std::string to_string_table()  const;"""

    hac_lines = [ '    public:', f'      /** HAC parameters of bank {upper_bnam}  */' ]
    var_lines = [ '    public:', f'      /** Data members of bank {upper_bnam}    */' ]
    acc_lines = [ '    public:', f'      /** Data accessors of bank {upper_bnam}  */' ]
    
    text_len  = 132;
    to_string_header = '  '
    to_string_format = '|  '
    to_string_data   = ''
    for i in range(len(variables)):
      v = variables[i]
      desc = ' Member:  '+str(i)
      it = v[0].replace('  ',' ').split(' ')
      items = []
      for l in it:
        if len(l):
          items.append(l)
      ##print( str(items) )
      array = None
      num   = items[0]
      
      if items[0].find('-') == -1:
        num   = int(items[0])
        num_words_bank = num
      else:
        it    = items[0].split('-')
        num   = int(it[0])
        num_words_bank = int(it[1])
        array = num_words_bank - num + 1

      snam   = items[1]
      typ    = items[2]
      lnam   = items[3]
      varnam = lnam[:2].lower() + lnam[2:]
      if varnam == 'class':
        varnam = 'clazz'

      data_range = ''
      if len(items) > 4:
        data_range = 'range:'+items[4]

      cxx_fmt    = ''
      cxx_data   = ''
      cxx_type   = typ
      if typ == 'I':
        cxx_type = 'int32_t'
        cxx_data = f'this->_{varnam}'
        cxx_fmt  = '%-10d |'
        defval   = '   {   0 };';
      elif typ == 'F':
        cxx_type = 'float'
        cxx_data = f'double(this->_{varnam})'
        cxx_fmt  = '%-8.3g |'
        defval   = '   { 0e0 };';
      elif typ == 'A':
        cxx_type = 'char'
        cxx_data = f'this->_{varnam}'
        cxx_fmt  = '%c%c%c%c |'
        defval   = ';';
        if array:
           array = array*4
        else:
           array = 4
      else:
        defval   = ';'

      dim = ''
      text_len = text_len + 32;
      access = cxx_type
      if array:
        defval   = None
        access   = cxx_type + '*'
        varnam   = f'{varnam}'
        dim      = f'dimension:{array}'
        cxx_data = f'this->_{varnam}[0]'
        if typ == 'A':
          cxx_fmt  = f' %-{array}s|'
          cxx_data = f'std::string(this->_{varnam},this->_{varnam}+{array}).c_str()'

      hac_param  = 'J' + bnam[:3]+snam + ' = ' + str(num-1)
      comment = f'{upper_bnam} Member: {lnam} of type {cxx_type} {data_range} {dim}'

      hac_lines.append(   '      constexpr static int32_t %s;'%(hac_param) )
      var_lines.append(   '      /// %-72s [%s]'%(comment, hac_param,) )
      if defval:
        var_lines.append( '      %-10s _%-18s %s'%(cxx_type, varnam, defval, ))
      elif array:
        var_lines.append( '      %-10s _%s[%s];'%(cxx_type, varnam, f'{array}', ))

      to_string_format = to_string_format + cxx_fmt
      if len(to_string_data):
        to_string_data = to_string_data + '\n               '
      to_string_data   = to_string_data + cxx_data + ','
      to_string_header = to_string_header + f'{lnam:11}|'

      const = ''
      access = ''
      if array:
        const = 'const '
        access = '*'
        lnam = f'{lnam}[{array}];'
      acc_lines.append( f'      /// Accessor to member data "_{varnam}"' )
      decl = f'{const}{cxx_type}{access} {varnam}()  const'
      impl = f'{bracket_open}  return this->_{varnam};'
      acc_lines.append( '      %-35s %-40s %s'%(decl, impl, bracket_close, ))

    # Correct for invalid sub-header definitions (LALI/LCAL)
    if num_words_per_bank == 0:
      num_words_per_bank = num_words_bank

    hac_lines.append( f'      constexpr static int32_t KC{upper_bnam} = {num_words_per_bank};' )
    text_len = ((text_len/64)+1)*64

    data_stringify_impl = '    return { };'
    if num_words_per_bank > 0: 
      data_stringify_impl = f"""char line[{int(text_len)}];
    line[0] = 0;
    ::snprintf(line, sizeof(line),
               "{to_string_format}",
               {to_string_data[:-1]} );
    return {bracket_open} line {bracket_close};"""

    table_stringify_impl = ''
    if bank_is_object_table:
      table_stringify_impl = f"""
  /// Stringified object data of bank type "{upper_bnam}"
  inline std::string {lower_bnam}::to_string_table()  const  {bracket_open} 
    const int* ptr = ((const int*)this) - 2;
    std::stringstream result;

    if( ptr[0] != {num_words_per_bank} )  {bracket_open}
      result << "Invalid ALEPH BOS table pointer: " << (void*)this;
      throw std::runtime_error(result.str());
    {bracket_close}
    result << this->to_string_header();
    ptr = (const int*)this;
    for(int i=0; i<ptr[1]; ++i, ptr += {num_words_per_bank} )  {bracket_open}
      const auto* bank = (const {lower_bnam}*)ptr;
      result << bank->to_string_data() << std::endl;
    {bracket_close}
  {bracket_close}

  /// Raw data structure for table entities
  using table_{lower_bnam} = object_table< {lower_bnam} , {lower_bnam} >;"""

    stringify_decl = ''
    stringify_impl = ''
    if self.have_stringifiers:
      stringify_decl = f"""
      {header_stringify_decl}
      {data_stringify_decl}\n{object_stringify_decl}{table_stringify_decl}"""
      
      stringify_impl = f"""
  /// Stringified object data of bank type "{upper_bnam}"
  inline std::string {lower_bnam}::to_string_data()  const  {bracket_open}
    {data_stringify_impl}
  {bracket_close}
  /// Stringified data header of bank type "{upper_bnam}"
  inline std::string {lower_bnam}::to_string_header()  const  {bracket_open}
    std::stringstream result;
    result << "+{123*'-'}" << std::endl;
    result << "| {to_string_header}" << std::endl;
    result << "+{123*'-'}" << std::endl;
    return result.str();
  {bracket_close}
  /// Stringified data header of bank type "{upper_bnam}"
  inline std::string {lower_bnam}::to_string()  const  {bracket_open}
    std::stringstream result;
    result << this->to_string_header()
           << this->to_string_data() << std::endl;
    return result.str();
  {bracket_close}
  {table_stringify_impl}"""

    self.output ( f"""//==========================================================================
//  Software for the ALEPH experiment
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see ALSOFTINSTALL/LICENSE.
// For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
// IMPORTANT NOTICE: This file is generated. Do not edit!
//==========================================================================
#ifndef ALPHA_BOSBANK_{upper_bnam}_H
#define ALPHA_BOSBANK_{upper_bnam}_H

/// C/C++ include files
#include <string>
#include <cstdint>
#include <sstream>

/// Framework include files
#include <alpha/defs.h>

/// alpha namespace declaration
namespace alpha  {bracket_open}

  /// Forward declarations
  template <typename DATA, typename PRESENTATION> class object_table;

  /// Definition of the BOS data structure {upper_bnam}, class {lower_bnam}
  /**
{to_string_list(comments, '    *   %s')}
    *   {header_match}
{to_string_list(subheader, '    *   %s')}
    *   {header_match}
{to_string_list(var_comments, '%s')}
    *
    *     \\author M.Frank
    *     \\version 1.0
    */
  class {lower_bnam}  {bracket_open}
{to_string_list(hac_lines)}
{to_string_list(var_lines)}

    private:
      /// Standard constructor (inhibited, object lives in BOS space)
      {lower_bnam}() = delete;
      /// Standard destructor (inhibited, object lives in BOS space)
      ~{lower_bnam}() = delete;
      
{to_string_list(acc_lines)}
{stringify_decl}
  {bracket_close};  // End of class definition 'class {lower_bnam}' (ALEPH Bank {upper_bnam})
{stringify_impl}
{bracket_close}     // End of namespace alpha
#endif /* ALPHA_BOSBANK_{upper_bnam}_H */""")

    return self.header_data

  # ========================================================================
  def preprocess(self, bnam, lines):
    """
         Extract header file information from ALEPH bank inventory
         \version 1.0
         \author  M.Frank
    """
    status = 0
    comments   = []
    subheader  = []
    variables  = []
    curr_var = None
    #    pdb.set_trace()
    for line in lines:
      hdr_match = line.find(header_match) != -1
      if status == 0 and not hdr_match:
        debug (f'---> comment:    {line}')
        comments.append(line)
      elif status == 0 and hdr_match:
        status = 1
        continue
      elif status <= 10 and line.find('          I    number of') > 0:
        debug (f'---> sub-header: {line}')
        subheader.append(line)
      elif status <= 10 and line.find('       # of ') > 0:
        debug (f'---> sub-header: {line}')
        subheader.append(line)
      elif len(line) < 7:
        continue
      elif line.find('                                Page: ') > 0:
        break
      elif status == 1 and len(line)>6 and line[6] != ' ' and not hdr_match:
        status = 100
        curr_var = []
        curr_var.append(line)
        debug (f'---> variable:   {line}')
      elif status >= 1 and status <= 10 and (len(line)>13 and line[13] == ' ' and line[5] != ' ' and not hdr_match):
        debug (f'---> sub-header: {line}')
        subheader.append(line)
        status = 10
      elif status >= 10 and hdr_match:
        status = 100
      elif status >= 100 and len(line)>6 and line[6] != ' ':
        if curr_var:
          variables.append(curr_var)
        curr_var = []
        curr_var.append(line)
        debug (f'---> variable:   {line}')
      elif status >= 100 and curr_var:
        curr_var.append(line)
        debug (f'---> variable:   {line}')
      else:
        debug (f'---> dropped:    {line}')
      

    if curr_var:
      variables.append(curr_var)
    return (bnam, comments, subheader, variables)

  # ========================================================================
  def generate(self, bnam, lines):
    bank_name, comments, subheader, variables = self.preprocess(bnam, lines)
    if bank_name:
      return self.gen_header(bank_name, comments, subheader, variables)
    else:
      print( f'ERROR: Bank {bnam} was not found in file {lbf_file}' )
      return None
  
def generate_header(bank_name, lines, stringifiers):
  bank_lines = []
  got_bank = False
  bmatch   = '| '+bank_name+' |'
  for i in range(len(lines)):
    line = lines[i][:-1]
    if line.find(bmatch) > 0:
      bank_lines.append(lines[i-1][:-1])
      bank_lines.append(line)
      got_bank = True
    elif got_bank and line.find(bend) == -1:
      bank_lines.append(line)
    elif got_bank and line.find(bend) == 0:
      gen = header_generator()
      if stringifiers:
        gen.have_stringifiers = stringifiers
      data = gen.generate(bank_name, bank_lines)
      return data

import pathlib
import argparse

bank_exclusions = ['HTTD', 'HWTD', 'ITDI', 'MUDT', 'VDDT', 'VDTD']

parser = argparse.ArgumentParser(allow_abbrev=False,
                                 prog='gen_header',
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 description='Generate header file from ALEPH bank definition',
                                 epilog='Usage example: '+__usage__+'\n\n'+
                                 'For these bank headers cannot be generated:\n  %s\n.\n'%\
                                 (str(bank_exclusions)[1:-1],) )

host   = 'phmf003'
#
#  Check debug flag
parser.add_argument(
  '-D',
  '--debug',
  action='store_true',
  dest='debug',
  default=False,
  help='Enable python debugging (invoke pdb)',
)
#
# LBF file location
parser.add_argument(
  '-l',
  '--lbf',
  type=str,
  dest='lbf_file',
  default='/home/frankm/Aleph/offline/doc/sbank.lbf',
  help='Location of the bank definition file',
)
#
#  Check to generate headers for all known banks
parser.add_argument(
  '-a',
  '--all',
  action='store_true',
  dest='generate_all',
  default=False,
  help='Generate header files for all known banks in the LBF definition file',
)
#
# Pass bank names for header generation
parser.add_argument(
  '-b',
  '--bank',
  type=str,
  dest='bank_name',
  default=None,
  help='Bank name for which headers should be generated. Seperate multiple names by comma',
)
#
# Output path
parser.add_argument(
  '-o',
  '--output',
  type=pathlib.Path,
  dest='output',
  default=None,
  help='Path to place the resulting header file(s)',
)
#
# Only show all banks available for header generation
parser.add_argument(
  '-s',
  '--show-banks',
  action='store_true',
  dest='show_banks',
  default=False,
  help='Show list containing all bank definitions in lbf-file',
)
#
# Generate headers with automated printout helpers of banks
parser.add_argument(
  '-S',
  '--stringifiers',
  action='store_true',
  dest='stringifiers',
  default=False,
  help='Generate headers with bank stringifiers (not always correct and nice)',
)
#
#
args = parser.parse_args()
#
#
if args.show_banks:
   print(all_banks.replace('\n',' ').replace('  ',' '))
   sys.exit(0)
elif args.generate_all:
   bank_names = all_banks.replace('\n',' ').replace('  ',' ')
else:
   bank_names = args.bank_name

lbf_file = args.lbf_file
banks = bank_names.replace(' ',',').split(',')
global_debug = args.debug
# pdb.set_trace()

if banks is not None:
  lines    = open( args.lbf_file, "r" ).readlines();
  for bank_name in banks:
    if bank_name in bank_exclusions:
      print( f'+++ CANNOT generate headers for bank type {bank_name}. IGNORED.' )
      continue
    header_data = generate_header(bank_name, lines, args.stringifiers)
    if not header_data:
      print( f'Failed to generate header: No bank {bank_name} found!' )
      #sys.exit(2)
    else:
      if args.output:
        if not os.path.exists(args.output):
          pit = str(args.output).split(os.sep)
          dir = pit[0]
          for p in pit[1:]:
            if not os.path.exists(dir):
              os.mkdir(dir)
            dir = dir + os.sep + p
            if not os.path.exists(dir):
              os.mkdir(dir)
        file_name = args.output / (bank_name.lower() + '.h')
        file = open(file_name,'w')
        file.write(header_data)
        file.close()
        print( f'Successfully write header file: {str(file_name)}' )
      else:
        print( f'{header_data}' )
else:
  print( f'No bank names passed for header file generation!' )
  parser.print_help()
