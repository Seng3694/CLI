set(INCLUDE_DIR
	include
)

set(HEADERS
	${INCLUDE_DIR}/common.h
	${INCLUDE_DIR}/cli.h
	${INCLUDE_DIR}/argument.h
)

set(SOURCES
	src/cli.c
	src/argument.c
)

add_library(${PROJECT_NAME} ${HEADERS} ${INLINES} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})