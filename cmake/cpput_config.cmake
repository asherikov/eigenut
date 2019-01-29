function(cpput_config INPUT OUTPUT)
    if(NOT EXISTS "${INPUT}")
        message(FATAL_ERROR "Input file not found.")
    endif()

    include (cmakeut_detect_func_macro)
    cmakeut_detect_func_macro()

    configure_file("${INPUT}" "${OUTPUT}")
endfunction(cpput_config)
