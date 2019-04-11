import deregress
import os

THIS_DIRECTORY = os.path.dirname(__file__)
PATH_TO_SSCRIPT = os.path.abspath(os.path.join(THIS_DIRECTORY, "../../source/sscript"))

@deregress.test
def arithmetic_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "arithmetic.sel"))) \
        .arg("--run") \
        .stdout("arithmetic.out.txt") \
        .run()

    test.tester.file_contents_should_match("arithmetic.out.txt")


@deregress.test
def arithmetic_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "arithmetic.sel"))) \
        .arg("--dump-ast") \
        .stdout("arithmetic.ast.txt") \
        .run()

    test.tester.file_contents_should_match("arithmetic.ast.txt")


@deregress.test
def classes_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "classes.sel"))) \
        .arg("--run") \
        .stdout("classes.out.txt") \
        .run()

    test.tester.file_contents_should_match("classes.out.txt")


@deregress.test
def classes_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "classes.sel"))) \
        .arg("--dump-ast") \
        .stdout("classes.ast.txt") \
        .run()

    test.tester.file_contents_should_match("classes.ast.txt")


@deregress.test
def comparison_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "comparison.sel"))) \
        .arg("--run") \
        .stdout("comparison.out.txt") \
        .run()

    test.tester.file_contents_should_match("comparison.out.txt")


@deregress.test
def comparison_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "comparison.sel"))) \
        .arg("--dump-ast") \
        .stdout("comparison.ast.txt") \
        .run()

    test.tester.file_contents_should_match("comparison.ast.txt")


@deregress.test
def control_flow_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "control_flow.sel"))) \
        .arg("--run") \
        .stdout("control_flow.out.txt") \
        .run()

    test.tester.file_contents_should_match("control_flow.out.txt")


@deregress.test
def control_flow_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "control_flow.sel"))) \
        .arg("--dump-ast") \
        .stdout("control_flow.ast.txt") \
        .run()

    test.tester.file_contents_should_match("control_flow.ast.txt")


@deregress.test
def fib_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "fib.sel"))) \
        .arg("--run") \
        .stdout("fib.out.txt") \
        .run()

    test.tester.file_contents_should_match("fib.out.txt")


@deregress.test
def fib_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "fib.sel"))) \
        .arg("--dump-ast") \
        .stdout("fib.ast.txt") \
        .run()

    test.tester.file_contents_should_match("fib.ast.txt")


@deregress.test
def logic_expressions_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "logic_expressions.sel"))) \
        .arg("--run") \
        .stdout("logic_expressions.out.txt") \
        .run()

    test.tester.file_contents_should_match("logic_expressions.out.txt")


@deregress.test
def logic_expressions_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "logic_expressions.sel"))) \
        .arg("--dump-ast") \
        .stdout("logic_expressions.ast.txt") \
        .run()

    test.tester.file_contents_should_match("logic_expressions.ast.txt")


@deregress.test
def functions_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "functions.sel"))) \
        .arg("--run") \
        .stdout("functions.out.txt") \
        .run()

    test.tester.file_contents_should_match("functions.out.txt")


@deregress.test
def functions_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "functions.sel"))) \
        .arg("--dump-ast") \
        .stdout("functions.ast.txt") \
        .run()

    test.tester.file_contents_should_match("functions.ast.txt")


@deregress.test
def variables_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "variables.sel"))) \
        .arg("--run") \
        .stdout("variables.out.txt") \
        .run()

    test.tester.file_contents_should_match("variables.out.txt")


@deregress.test
def variables_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "variables.sel"))) \
        .arg("--dump-ast") \
        .stdout("variables.ast.txt") \
        .run()

    test.tester.file_contents_should_match("variables.ast.txt")


@deregress.test
def anonymous_methods_execution(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "anonymous_methods.sel"))) \
        .arg("--run") \
        .stdout("anonymous_methods.out.txt") \
        .run()

    test.tester.file_contents_should_match("anonymous_methods.out.txt")


@deregress.test
def anonymous_methods_ast(test):
    test.runner.executable(PATH_TO_SSCRIPT) \
        .arg(os.path.abspath(os.path.join(THIS_DIRECTORY, "anonymous_methods.sel"))) \
        .arg("--dump-ast") \
        .stdout("anonymous_methods.ast.txt") \
        .run()

    test.tester.file_contents_should_match("anonymous_methods.ast.txt")


