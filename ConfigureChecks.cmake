################################################################################
#  This file is released under the GNU GPL version 3 or later.                 #
################################################################################


# required stuff
find_package( TQt )
find_package( TDE )

tde_setup_architecture_flags( )

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

tde_setup_largefiles( )


##### check for gcc visibility support

if( WITH_GCC_VISIBILITY )
  tde_setup_gcc_visibility( )
endif( WITH_GCC_VISIBILITY )

