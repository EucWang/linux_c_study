project(linux_study)
cmake_minimum_required(VERSION 3.0)
#aux_source_directory(. SRC_LIST)
set(SOURCE_FILES
    main.c
    test.h
    6_ipc/17_semop.test.c
    6_ipc/19_shm_client.test.c
    6_ipc/19_shm_server.test.c
    6_ipc/20_shm_test.c

    6_complex_example/21.myshm.h
    6_complex_example/21.myshm.c
    6_complex_example/21.shm.client.c
    6_complex_example/21.shm.server.c

    7_thread/7.1.pthread_create.test.c
    7_thread/7.2.pthread_join.test.c
    7_thread/7.3.pthread_mutex.test.c
    7_thread/7.4.pthread_cond.test.c
    7_thread/7.5.pthread_cleanup.test.c
    7_thread/7.6.pthread_specific.test.c
    7_thread/7.7.pthread_attr.test.c

    8_network/8.1.bite_sequence.test.c
    8_network/8.2.server.test.c
    8_network/8.3.client.test.c
    8_network/8.5.send.oob.test.c
    8_network/8.6.recv.oob.test.c
    )
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} pthread)

