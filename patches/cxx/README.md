
** Create DD4hep geometry from Geant3

- KINGALCARDS=${ALEPH_BUILD_DIR}/cards/pyth05.cards ${ALEPH_BUILD_DIR}/build64/pyth05
- GALEPHCARDS=${ALEPH_BUILD_DIR}/cards/galeph.cards galeph_geom>aleph.geometry.txt
- python ../galeph/extract_geometry.py -I aleph.geometry.txt -O ../cxx/dd4hep/aleph_geometry.cpp

