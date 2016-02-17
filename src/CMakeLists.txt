set(COMMON_NAME "${PROJECT_NAME}_common")

# Build common files
add_subdirectory(common)

# Build client
add_subdirectory(client)

# Build server
add_subdirectory(server)

# Build chat
add_subdirectory(chat)
