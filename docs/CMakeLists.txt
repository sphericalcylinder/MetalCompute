set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_GENERATE_HTML YES)

doxygen_add_docs(
    docs
    "${MetalCompute_SOURCE_DIR}/src"
    "${MetalCompute_SOURCE_DIR}/docs"
    ALL
    COMMENT "Generate doxygen documentation"
)
