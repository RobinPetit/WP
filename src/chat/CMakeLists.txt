# For now there is no file to compile for the chat other than chat.cpp
# But as soon there is one or more, uncomment and add them here
#add_library(${CHAT_LIB_NAME}
#		)

set(CHAT_NAME "${PROJECT_NAME}_chat")

add_executable(${CHAT_NAME} "chat.cpp")

# Link the executable to the libraries
target_link_libraries(${CHAT_NAME}
	${CHAT_LIB_NAME}
	${COMMON_LIB_NAME}
	${EXTERNAL_LIBRARIES}
	pthread)
