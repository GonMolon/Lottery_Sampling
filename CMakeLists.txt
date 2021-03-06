cmake_minimum_required(VERSION 3.10)
project(LotterySampling)


###################################################
# Compiler settings
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")

foreach(flags
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_MINSIZEREL)
        string(REGEX REPLACE "(^| )[/-]D *NDEBUG($| )" " " "${flags}" "${${flags}}")
endforeach()


#set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3")
#set(CMAKE_CXX_FLAGS_RELEASEWITHASSERTS "-Wall -O3")


###################################################
# Source files
set(SOURCE_FILES
        src/Main.cpp
        src/utils/InputParser.h
        src/utils/InputParser.cpp
        src/utils/Stats.h
        src/utils/Stats.ipp
        src/utils/Misc.h
        src/utils/TicketUtils.h
        src/utils/TicketUtils.cpp
        src/algorithms/GenericAlgorithm.h
        src/algorithms/GenericAlgorithm.ipp
        src/algorithms/lottery_sampling/Algorithm.h
        src/algorithms/lottery_sampling/Algorithm.ipp
        src/algorithms/lottery_sampling/Types.h
        src/algorithms/lottery_sampling_hh/Algorithm.h
        src/algorithms/lottery_sampling_hh/Algorithm.ipp
        src/algorithms/lottery_sampling_hh/Types.h
        src/algorithms/lottery_sampling_v2/Algorithm.h
        src/algorithms/lottery_sampling_v2/Algorithm.ipp
        src/algorithms/lottery_sampling_v2/Types.h
        src/algorithms/lottery_sampling_v2_b/Algorithm.h
        src/algorithms/lottery_sampling_v2_b/Algorithm.ipp
        src/algorithms/lottery_sampling_v2_b/Types.h
        src/algorithms/lottery_sampling_v2_c/Algorithm.h
        src/algorithms/lottery_sampling_v2_c/Algorithm.ipp
        src/algorithms/lottery_sampling_v2_c/Types.h
        src/algorithms/basic_lottery_sampling/Algorithm.h
        src/algorithms/basic_lottery_sampling/Algorithm.ipp
        src/algorithms/basic_lottery_sampling/Types.h
        src/algorithms/basic_lottery_sampling_hh/Algorithm.h
        src/algorithms/basic_lottery_sampling_hh/Algorithm.ipp
        src/algorithms/basic_lottery_sampling_hh/Types.h
        src/algorithms/basic_lottery_sampling_top_k/Algorithm.h
        src/algorithms/basic_lottery_sampling_top_k/Algorithm.ipp
        src/algorithms/basic_lottery_sampling_top_k/Types.h
        src/algorithms/lottery_sampling_parallel/Algorithm.h
        src/algorithms/lottery_sampling_parallel/Algorithm.ipp
        src/algorithms/lottery_sampling_parallel/Types.h
        src/algorithms/space_saving/Algorithm.h
        src/algorithms/space_saving/Algorithm.ipp
        src/algorithms/space_saving/Types.h
        src/algorithms/lossy_counting/Algorithm.h
        src/algorithms/lossy_counting/Algorithm.ipp
        src/algorithms/lossy_counting/Types.h
        src/algorithms/sticky_sampling/Algorithm.h
        src/algorithms/sticky_sampling/Algorithm.ipp
        src/algorithms/sticky_sampling/Types.h
        src/algorithms/frequent/Algorithm.h
        src/algorithms/frequent/Algorithm.ipp
        src/algorithms/frequent/Types.h
        src/algorithms/count_sketch/Algorithm.h
        src/algorithms/count_sketch/Algorithm.ipp
        src/algorithms/count_sketch/Types.h
        src/algorithms/freq_obs/Algorithm.h
        src/algorithms/freq_obs/Algorithm.ipp
        src/algorithms/freq_obs/Types.h
        src/data_structures/BinaryHeap.h
        src/data_structures/BinaryHeap.ipp
        src/data_structures/SortedTree.h
        src/data_structures/SortedTree.ipp
        src/data_structures/SortedList.h
        src/data_structures/SortedList.ipp
        src/data_structures/SortedVector.h
        src/data_structures/SortedVector.ipp
        )


###################################################
# Executable
add_executable(heavy_hitters ${SOURCE_FILES})
target_include_directories(heavy_hitters PUBLIC src)


###################################################
# Tests
enable_testing()

add_test(
        NAME
        MemoryLeak
        COMMAND
        python3 main.py MemoryLeak
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

