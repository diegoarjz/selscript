set(REGRESSION_TEST_SCRIPTS
    "anonymous_methods.sel"
    "arithmetic.sel"
    "classes.sel"
    "comparison.sel"
    "control_flow.sel"
    "fib.sel"
    "functions.sel"
    "logic_expressions.sel"
    "variables.sel"
    "regression_test.py"
)

foreach (file ${REGRESSION_TEST_SCRIPTS})
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach()
