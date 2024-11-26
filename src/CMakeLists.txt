file(GLOB_RECURSE src_f CONFIGURE_DEPENDS
    "*.hpp"
    "*.h"
    "*.cpp"
    "*.c"
)
list(FILTER src_f EXCLUDE REGEX ".*example1.cpp")

target_sources(aeregui PRIVATE
    ${src_f}
)

target_sources(example1 PRIVATE
    "src/example1.cpp"
)
