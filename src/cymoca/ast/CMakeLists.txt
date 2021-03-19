add_library(cymoca_ast STATIC
        ast.cc
        ast.h
        condition/condition.h
        element/element.h
        equation/equation.h
        expression/expression.h
        listener/lisp.h
        listener/listener.h
        model/model.h
        node.h
        statement/statement.h
        walker.h
		)

target_include_directories(cymoca_ast SYSTEM PUBLIC
        ${CYMOCA_BINARY_DIR}/src
        ${Antlr4Runtime_INCLUDE_DIRS}
        )

target_include_directories(cymoca_ast PUBLIC
        ${CYMOCA_SOURCE_DIR}/src
        )

target_compile_options(cymoca_ast PUBLIC
        -pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)
