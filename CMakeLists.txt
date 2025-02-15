cmake_minimum_required(VERSION 3.13)

# use <PackageName>_ROOT variables
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
# honor CMAKE_REQUIRED_LIBRARIES in the include file check macros
if(POLICY CMP0075)
  cmake_policy(SET CMP0075 NEW)
endif()

project(SeisSol LANGUAGES C CXX)

# TODO: Move one dir up.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake"
                      "${PROJECT_SOURCE_DIR}/submodules/Device/cmake"
                      "${CMAKE_ROOT}/Modules")

if (CMAKE_CXX_COMPILER_ID MATCHES "NVHPC|PGI")
  set(NVHPC_REQUIRED_VERSION "22.1.0")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS NVHPC_REQUIRED_VERSION)
    message(FATAL_ERROR "NVHPC version ${NVHPC_REQUIRED_VERSION} or higher is required")
  endif()
endif()

# set hardware specific definition needed for seissol compilation
# 'process_users_input' returns the following:
#
#       switches: HDF5, NETCDF, GRAPH_PARTITIONING_LIBS, MPI, OPENMP, ASAGI, MEMKIND,
#                 PROXY_PYBINDING, ENABLE_PIC_COMPILATION, PREMULTIPLY_FLUX
#
#       user's input: HOST_ARCH, DEVICE_ARCH, DEVICE_SUB_ARCH,
#                     ORDER, NUMBER_OF_MECHANISMS, EQUATIONS,
#                     PRECISION, DYNAMIC_RUPTURE_METHOD,
#                     NUMBER_OF_FUSED_SIMULATIONS,
#                     MEMORY_LAYOUT,
#                     LOG_LEVEL, LOG_LEVEL_MASTER,
#                     GEMM_TOOLS_LIST, EXTRA_CXX_FLAGS,
#                     DR_QUAD_RULE
#       derived input: REAL_SIZE_IN_BYTES, ALIGNMENT, VECTORSIZE ARCH_STRING, NUMBER_OF_QUANTITIES, WITH_GPU, DEVICE_VENDOR
#
include(cmake/process_users_input.cmake)
set(HARDWARE_DEFINITIONS "ALIGNMENT=${ALIGNMENT}"
                         "VECTORSIZE=${VECTORSIZE}"
                         "REAL_SIZE=${REAL_SIZE_IN_BYTES}"
                         "ALIGNED_REAL_SIZE=${REAL_SIZE_IN_BYTES}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo") # MinSizeRel is useless for us
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Set build type to Release as none was supplied.")
endif()

if (NOT (DEVICE_BACKEND STREQUAL "hip" AND DEVICE_ARCH MATCHES "sm_*"))
  # Note: hip 4.x has a bug during setting rpath while compiling
  #       device code for nvidia gpus. This scenario is used in
  #       SeisSol's GPU CI pipeline.
  set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
endif()

# Generate version.h

include(GetGitRevisionDescription)

# get GIT info
git_describe(PACKAGE_GIT_VERSION --tags --always --dirty=\ \(dirty\) --broken=\ \(broken\))
if (${PACKAGE_GIT_VERSION} MATCHES "NOTFOUND")
  set(PACKAGE_GIT_VERSION "(unknown)")
  set(PACKAGE_GIT_HASH "(unknown)")
  set(PACKAGE_GIT_TIMESTAMP "9999-12-31T00:00:00+00:00")
else()
  get_git_commit_info(PACKAGE_GIT_HASH PACKAGE_GIT_TIMESTAMP)
endif()
string(SUBSTRING ${PACKAGE_GIT_TIMESTAMP} 0 4 PACKAGE_GIT_YEAR)

# write file and print info
configure_file("src/version_template.h"
               "${CMAKE_CURRENT_BINARY_DIR}/src/version.h")
message(STATUS "Version: " ${PACKAGE_GIT_VERSION})
message(STATUS "Last commit: ${PACKAGE_GIT_HASH} at ${PACKAGE_GIT_TIMESTAMP}")

add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/src/generated_code")

set(GENERATED_FILES_FOR_SEISSOL src/generated_code/subroutine.h
                                src/generated_code/tensor.cpp
                                src/generated_code/subroutine.cpp
                                src/generated_code/gpulike_subroutine.cpp
                                src/generated_code/tensor.h
                                src/generated_code/init.cpp
                                src/generated_code/init.h
                                src/generated_code/kernel.h
                                src/generated_code/kernel.cpp)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/generated_code/)

if(PROXY_PYBINDING)
  find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
else()
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
endif()

if(PREMULTIPLY_FLUX)
  set(PREMULTIPLY_FLUX_VALUE "--enable_premultiply_flux")
else()
  set(PREMULTIPLY_FLUX_VALUE "--disable_premultiply_flux")
endif()

find_package(GemmTools REQUIRED)

add_custom_command(
  COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/generated_code/generate.py"
     "--equations" ${EQUATIONS}
     "--matricesDir" ${CMAKE_CURRENT_SOURCE_DIR}/generated_code/matrices
     "--outputDir" ${CMAKE_CURRENT_BINARY_DIR}/src/generated_code
     "--host_arch" ${HOST_ARCH_STR}
     "--device_arch" ${DEVICE_ARCH_STR}
     "--device_backend" ${DEVICE_BACKEND}
     "--order" ${ORDER}
     "--numberOfMechanisms" ${NUMBER_OF_MECHANISMS}
     "--memLayout" ${MEMORY_LAYOUT}
     "--multipleSimulations" ${NUMBER_OF_FUSED_SIMULATIONS}
     "--PlasticityMethod" ${PLASTICITY_METHOD}
     "--gemm_tools" ${GEMM_TOOLS_LIST}
     "--drQuadRule" ${DR_QUAD_RULE}
     "--executable_libxsmm=${Libxsmm_executable_PROGRAM}"
     "--executable_pspamm=${PSpaMM_PROGRAM}"
     ${PREMULTIPLY_FLUX_VALUE} # boolean flag
     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/generated_code
     DEPENDS
        build-time-make-directory
        generated_code/arch.py
        generated_code/memlayout.py
        generated_code/viscoelastic.py
        generated_code/DynamicRupture.py
        generated_code/Plasticity.py
        generated_code/multSim.py
        generated_code/aderdg.py
        generated_code/generate.py
        generated_code/Point.py
        generated_code/elastic.py
        generated_code/viscoelastic2.py
        generated_code/poroelastic.py
        generated_code/anisotropic.py
        generated_code/SurfaceDisplacement.py
        generated_code/NodalBoundaryConditions.py
        generated_code/matrices
    OUTPUT
        ${GENERATED_FILES_FOR_SEISSOL}
    COMMENT "Codegen for tensor stuff."
       )

add_custom_target(SeisSol-codegen ALL DEPENDS ${GENERATED_FILES_FOR_SEISSOL})
add_library(SeisSol-common-properties INTERFACE IMPORTED)

include(src/sources.cmake)

# enforce code generation to run before any other target
add_dependencies(SeisSol-lib SeisSol-codegen)

target_link_libraries(SeisSol-lib PUBLIC SeisSol-common-properties)
if (WITH_GPU)
  add_dependencies(SeisSol-device-lib SeisSol-codegen)
endif()


if(GemmTools_INCLUDE_DIRS)
  target_include_directories(SeisSol-common-properties INTERFACE ${GemmTools_INCLUDE_DIRS})
endif()
if(GemmTools_LIBRARIES)
  target_link_libraries(SeisSol-common-properties INTERFACE ${GemmTools_LIBRARIES})
endif()
if(GemmTools_COMPILER_DEFINITIONS)
  target_compile_definitions(SeisSol-common-properties INTERFACE ${GemmTools_COMPILER_DEFINITIONS})
endif()

# Find appropriate compiler flags based on the target computer architecture
# and the target compiler
# 'get_arch_specific_flags' returns: CPU_ARCH_FLAGS
include(cmake/cpu_arch_flags.cmake)
get_arch_flags(${HOST_ARCH} ${CMAKE_CXX_COMPILER_ID})

# set hardware/compiler specific definitions and flags
target_compile_definitions(SeisSol-common-properties INTERFACE ${HARDWARE_DEFINITIONS})
target_compile_options(SeisSol-common-properties INTERFACE ${CPU_ARCH_FLAGS})

target_compile_definitions(SeisSol-common-properties INTERFACE LOGLEVEL=${LOG_LEVEL})
target_compile_definitions(SeisSol-common-properties INTERFACE LOG_LEVEL=${LOG_LEVEL_MASTER}
                                                     LOGLEVEL0=${LOG_LEVEL_MASTER})


if (PLASTICITY_METHOD STREQUAL "ip")
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_PLASTICITY_IP)
elseif (PLASTICITY_METHOD STREQUAL "nb")
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_PLASTICITY_NB)
endif()


# enable interproc. opts for small cores
#if cpu in ['knc', 'knl', 'skx']:
#    flags.extend(['-ip'])

# Libs
include(ExternalProject)

find_package(easi 1.0.0 REQUIRED)
target_link_libraries(SeisSol-common-properties INTERFACE easi::easi)

if (OPENMP)
  find_package(OpenMP REQUIRED)
  target_link_libraries(SeisSol-common-properties INTERFACE OpenMP::OpenMP_CXX)
  target_compile_definitions(SeisSol-common-properties INTERFACE OMP OMPI_SKIP_MPICXX)
endif()

if (MPI)
  find_package(MPI REQUIRED)

  target_include_directories(SeisSol-common-properties SYSTEM INTERFACE ${MPI_CXX_INCLUDE_PATH})
  target_link_libraries(SeisSol-common-properties INTERFACE MPI::MPI_C)

  target_compile_definitions(SeisSol-common-properties INTERFACE USE_MPI PARALLEL)
endif()

if (NUMA_AWARE_PINNING)
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_NUMA_AWARE_PINNING)
  find_package(NUMA REQUIRED)

  target_include_directories(SeisSol-common-properties SYSTEM INTERFACE ${NUMA_INCLUDE_DIR})
  target_link_libraries(SeisSol-common-properties INTERFACE ${NUMA_LIBRARY})
endif()

#set(HDF5_PREFER_PARALLEL True)
if (NETCDF)
  find_package(NetCDF REQUIRED)
  target_include_directories(SeisSol-common-properties INTERFACE ${NetCDF_INCLUDE_DIRS})
  target_link_libraries(SeisSol-common-properties INTERFACE ${NetCDF_LIBRARY})
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_NETCDF)
endif()

if (HDF5)
  if (MPI)
    set(HDF5_PREFER_PARALLEL True)
  endif()
  find_package(HDF5 REQUIRED COMPONENTS C HL)
  target_include_directories(SeisSol-common-properties INTERFACE ${HDF5_INCLUDE_DIRS})
  target_link_libraries(SeisSol-common-properties INTERFACE ${HDF5_C_HL_LIBRARIES} ${HDF5_C_LIBRARIES})
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_HDF)
endif()  

# Parmetis
if ("parmetis" IN_LIST GRAPH_PARTITIONING_LIBS)
  find_package(METIS REQUIRED)
  if (NOT METIS_64_BIT_INTEGER)
      message(WARNING "Found METIS compiled with IDXTYPEWIDTH = 32. It is strongly recommend to compile METIS with IDXTYPEWIDTH = 64, because otherwise the partitioning of large meshes might fail.")
  endif()
  find_package(ParMETIS REQUIRED)
  target_include_directories(SeisSol-common-properties INTERFACE ${PARMETIS_INCLUDE_DIRS})
  target_link_libraries(SeisSol-common-properties INTERFACE ${PARMETIS_LIBRARIES})
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_METIS USE_PARMETIS)
endif()
if ("parhip" IN_LIST GRAPH_PARTITIONING_LIBS)
    find_package(ParHIP REQUIRED)
    target_link_libraries(SeisSol-common-properties INTERFACE ParHIP::ParHIP)
    target_compile_definitions(SeisSol-common-properties INTERFACE USE_PARHIP)
endif()
if ("ptscotch" IN_LIST GRAPH_PARTITIONING_LIBS)
    find_package(SCOTCH REQUIRED)
    target_link_libraries(SeisSol-common-properties INTERFACE SCOTCH::ptscotch SCOTCH::ptscotcherr)
    target_compile_definitions(SeisSol-common-properties INTERFACE USE_PTSCOTCH)
endif()
if (NOT GRAPH_PARTITIONING_LIBS)
    message(WARNING "Compiling without graph partitioning library; expect poor performance in multi-rank runs.")
endif()

find_package(PkgConfig REQUIRED) 
if (ASAGI)
  # todo warn if netcdf is off
  pkg_check_modules(ASAGI REQUIRED IMPORTED_TARGET asagi) # asagi_nompi?
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_ASAGI)
  target_link_libraries(SeisSol-common-properties INTERFACE ${ASAGI_LDFLAGS})
  target_include_directories(SeisSol-common-properties INTERFACE ${ASAGI_INCLUDE_DIRS})
  target_compile_options(SeisSol-common-properties INTERFACE ${ASAGI_CFLAGS} ${ASAGI_CFLAGS_OTHER})
endif()

if (MEMKIND)
  find_package(Memkind REQUIRED)
  target_include_directories(SeisSol-common-properties INTERFACE ${MEMKIND_INCLUDE_DIR})
  target_link_libraries(SeisSol-common-properties INTERFACE ${MEMKIND_LIBRARIES})
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_MEMKIND)
endif()

if(${EQUATIONS} STREQUAL "poroelastic")
  include(CheckLanguage)
  check_language(Fortran)
  if(CMAKE_Fortran_COMPILER)
    enable_language(Fortran)
    include(FortranCInterface)
    FortranCInterface_HEADER(FC.h MACRO_NAMESPACE "FC_")
    find_package(LAPACK REQUIRED)
    target_include_directories(SeisSol-lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(SeisSol-lib PUBLIC ${LAPACK_LIBRARIES})
  else()
    message(FATAL_ERROR "SeisSol needs a Fortran compiler.")
  endif()
endif()


if (INTEGRATE_QUANTITIES)
  target_compile_definitions(SeisSol-common-properties INTERFACE INTEGRATE_QUANTITIES)
endif()

if (ADDRESS_SANITIZER_DEBUG)
  target_link_libraries(SeisSol-common-properties INTERFACE debug
          -fno-omit-frame-pointer -fsanitize=address -fsanitize-recover=address -static-libasan
  )
endif()

# Note: it is better to include `async` as
# system headers because they emit lot's of warnings
# from clang. Most of them are issues with respect
# to overriden virtual methods
target_include_directories(SeisSol-common-properties SYSTEM INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/submodules/async
)

find_package(Eigen3 3.4 REQUIRED)
find_package(yaml-cpp REQUIRED)
target_link_libraries(SeisSol-common-properties INTERFACE Eigen3::Eigen yaml-cpp)

# Note: eigen3 enables cuda support in its headers by default.
# The problem happens in `eigen3/Eigen/src/Core/util/Meta.h` while
# including `math_constants.h`. This header file is located in
# a different sub-folder in case of ROCm HIP. SeisSol does not
# use eigen in GPU kernels. Therefore, it is better to change
# the default behaviour (which assumes CUDA support) and
# set `EIGEN_NO_CUDA`
target_compile_definitions(SeisSol-common-properties INTERFACE EIGEN_NO_CUDA)

if (EIGEN3_INCLUDE_DIR)
  target_include_directories(SeisSol-common-properties INTERFACE ${EIGEN3_INCLUDE_DIR})
endif()

if (YAML_CPP_INCLUDE_DIR AND EXISTS "${YAML_CPP_INCLUDE_DIR}")
  target_include_directories(SeisSol-common-properties INTERFACE ${YAML_CPP_INCLUDE_DIR})
endif()

target_include_directories(SeisSol-common-properties INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/yateto/include
    ${CMAKE_CURRENT_BINARY_DIR}/src/
)

target_compile_definitions(SeisSol-common-properties INTERFACE
    CONVERGENCE_ORDER=${ORDER}
    NUMBER_OF_QUANTITIES=${NUMBER_OF_QUANTITIES}
    NUMBER_OF_RELAXATION_MECHANISMS=${NUMBER_OF_MECHANISMS}
    ${DR_QUAD_RULE}
)

if (PREMULTIPLY_FLUX)
  target_compile_definitions(SeisSol-common-properties INTERFACE USE_PREMULTIPLY_FLUX)
endif()

# adjust prefix name of executables
if ("${DEVICE_ARCH_STR}" STREQUAL "none")
  set(EXE_NAME_PREFIX "${CMAKE_BUILD_TYPE}_${HOST_ARCH_STR}_${ORDER}_${EQUATIONS}")
else()
  set(EXE_NAME_PREFIX "${CMAKE_BUILD_TYPE}_${DEVICE_ARCH_STR}_${DEVICE_BACKEND}_${ORDER}_${EQUATIONS}")
endif()

if (WITH_GPU)
  # set SeisSol GPU definitions
  target_compile_definitions(SeisSol-common-properties INTERFACE ACL_DEVICE)

  # add device submodule
  add_subdirectory(submodules/Device)
  target_include_directories(SeisSol-common-properties INTERFACE submodules/Device)
  target_link_libraries(SeisSol-common-properties INTERFACE device)

  # add SeisSol GPU part
  target_link_libraries(SeisSol-common-properties INTERFACE SeisSol-device-lib)

  add_library(general-sycl-offloading SHARED
      ${SYCL_DEPENDENT_SRC_FILES}
      ${SYCL_ONLY_SRC_FILES})
  target_link_libraries(general-sycl-offloading PRIVATE SeisSol-common-properties)

  set(SYCL_CC "hipsycl" CACHE STRING "The SYCL Compiler used for general SYCL offloading")
  set(SYCL_CC_OPTIONS hipsycl dpcpp)
  set_property(CACHE SYCL_CC PROPERTY STRINGS ${SYCL_CC_OPTIONS})

  if (SYCL_CC MATCHES "hipsycl")
    message(STATUS "Using HipSYCL for general SYCL offloading")
    # add GPU dynamic rupture target
    # always override HIPSYCL_TARGETS, as we may need to override it when the architecture changes
    if (DEVICE_ARCH MATCHES "sm_*")
      if (CMAKE_CXX_COMPILER_ID MATCHES "NVHPC|PGI")
        set(SYCL_USE_NVHPC_DEFAULT ON)
      else()
        set(SYCL_USE_NVHPC_DEFAULT OFF)
      endif()
      option(SYCL_USE_NVHPC "For Nvidia GPUs, use nvhpc instead of CUDA/nvcc." ${SYCL_USE_NVHPC_DEFAULT})
      if (SYCL_USE_NVHPC)
        # we assume that hipsycl was compiled with nvhpc compiler collection
        string(REPLACE sm_ cc NVCPP_ARCH ${DEVICE_ARCH})
        set(HIPSYCL_TARGETS "cuda-nvcxx:${NVCPP_ARCH}" CACHE STRING "" FORCE)
      else()
        set(HIPSYCL_TARGETS "cuda:${DEVICE_ARCH}" CACHE STRING "" FORCE)
        target_compile_options(general-sycl-offloading PRIVATE -Wno-unknown-cuda-version)
      endif()
      target_compile_definitions(general-sycl-offloading PRIVATE SYCL_PLATFORM_NVIDIA)
    elseif(DEVICE_ARCH MATCHES "gfx*")
      set(HIPSYCL_TARGETS "hip:${DEVICE_ARCH}" CACHE STRING "" FORCE)
      target_compile_definitions(general-sycl-offloading PRIVATE SYCL_PLATFORM_AMD)
    else()
      set(HIPSYCL_TARGETS "${DEVICE_BACKEND}:${DEVICE_ARCH}" CACHE STRING "" FORCE)
      target_compile_definitions(general-sycl-offloading PRIVATE SYCL_PLATFORM_Intel)
    endif()

    find_package(hipSYCL REQUIRED)

    # note, we need to add openmp as a link option explicitly for some compilers.
    # it is probably an issue of `add_sycl_to_target` macro
    target_link_options(general-sycl-offloading PRIVATE ${OpenMP_CXX_FLAGS})
    add_sycl_to_target(TARGET general-sycl-offloading SOURCES ${SYCL_ONLY_SRC_FILES})
  else()
    find_package(DpcppFlags REQUIRED)
    message(STATUS "Using DPC++ for general SYCL offloading")
    target_link_libraries(general-sycl-offloading PRIVATE dpcpp::device_flags)
    target_link_libraries(SeisSol-common-properties INTERFACE dpcpp::interface)
  endif()

  add_dependencies(general-sycl-offloading SeisSol-codegen)

  set_target_properties(general-sycl-offloading PROPERTIES OUTPUT_NAME "SeisSol_offloading_${EXE_NAME_PREFIX}")
  set_target_properties(SeisSol-device-lib PROPERTIES OUTPUT_NAME "SeisSol_gpucodegen_${EXE_NAME_PREFIX}")
  set_target_properties(device PROPERTIES OUTPUT_NAME "device_${CMAKE_BUILD_TYPE}_${DEVICE_ARCH_STR}_${DEVICE_BACKEND}")

  # set up GPU install targets
  install(TARGETS general-sycl-offloading LIBRARY)
  install(TARGETS SeisSol-device-lib LIBRARY)
  install(TARGETS device LIBRARY)
else()
  # add DR source files to SeisSol-lib if we are compiling without any GPU support
  target_sources(SeisSol-lib PRIVATE ${SYCL_DEPENDENT_SRC_FILES})
endif()

# C++ compiler settings
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(SeisSol-lib INTERFACE -pedantic $<$<OR:$<COMPILE_LANGUAGE:CXX>,$<COMPILE_LANGUAGE:C>>:-Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas>)

  # using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
  # todo remove std?, is ffreestanding needed?
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

  # Activate interprocedual optimization.
  #set_property(TARGET SeisSol-lib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE) 
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang|IntelLLVM")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "NVHPC|PGI")
    # warning 186 - pointless comparison of unsigned integer with zero detected
    # warning 612 - overloaded virtual function is only partially overridden
    # warning 111 - statement is unreachable
    set(WARNINGS "--display_error_number --diag_suppress186 --diag_suppress612 --diag_suppress111")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "22.3.1")
      set(WARNINGS "${WARNINGS} --diag_suppress1")
    endif()

    # NOTE:
    # 1. --pending_instantiations=0 allows an infinite recursive template instantiation
    # 2. EIGEN_DONT_VECTORIZE=1 waiting for eigen3 support for nvhpc compiler collection w.r.t. vectorization
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc,--pending_instantiations=0 ${WARNINGS} -DEIGEN_DONT_VECTORIZE=0")
endif()

find_package(FILESYSTEM REQUIRED)
target_link_libraries(SeisSol-common-properties INTERFACE std::filesystem)

# Generated code does only work without red-zone.
if (HAS_REDZONE)
  set_source_files_properties(
      ${CMAKE_CURRENT_BINARY_DIR}/src/generated_code/subroutine.cpp PROPERTIES COMPILE_FLAGS -mno-red-zone
  )
endif()

add_executable(SeisSol-bin src/main.cpp)
set_target_properties(SeisSol-bin PROPERTIES OUTPUT_NAME "SeisSol_${EXE_NAME_PREFIX}")
target_link_libraries(SeisSol-bin PUBLIC SeisSol-lib)

# SeisSol proxy-core
add_library(SeisSol-proxy-core auto_tuning/proxy/src/proxy_seissol.cpp)
target_link_libraries(SeisSol-proxy-core PUBLIC SeisSol-lib)

if (PROXY_PYBINDING)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    find_package(pybind11 2.6.2 REQUIRED)
  else()
    find_package(pybind11 2.3.0 REQUIRED)
  endif()

  pybind11_add_module(seissol_proxy_bindings auto_tuning/proxy/src/proxy_bindings.cpp)
  target_link_libraries(seissol_proxy_bindings PUBLIC SeisSol-proxy-core)
  file(COPY auto_tuning/proxy/src/proxy-runners DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()


if (WITH_GPU)
  target_link_libraries(SeisSol-bin PUBLIC general-sycl-offloading)
  target_link_libraries(SeisSol-proxy-core PUBLIC general-sycl-offloading)
endif()

# C/C++ proxy interface
add_executable(SeisSol-proxy auto_tuning/proxy/src/proxy_main.cpp)
target_link_libraries(SeisSol-proxy PUBLIC SeisSol-proxy-core SeisSol-lib)
set_target_properties(SeisSol-proxy PROPERTIES OUTPUT_NAME "SeisSol_proxy_${EXE_NAME_PREFIX}")
install(TARGETS SeisSol-proxy RUNTIME)

if (LIKWID)
  find_package(likwid REQUIRED)
  target_compile_definitions(SeisSol-proxy-core PUBLIC LIKWID_PERFMON)
  target_include_directories(SeisSol-proxy-core PUBLIC ${LIKWID_INCLUDE_DIR})
  target_link_libraries(SeisSol-proxy-core PUBLIC likwid::likwid)
  target_compile_definitions(SeisSol-lib PUBLIC LIKWID_PERFMON)
  target_include_directories(SeisSol-lib PUBLIC ${LIKWID_INCLUDE_DIR})
  target_link_libraries(SeisSol-lib PUBLIC likwid::likwid)
endif()

if (TESTING)
  enable_testing()
  include(cmake/doctest.cmake)

  # Coverage
  if(COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    include(cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    setup_target_for_coverage_lcov(
            NAME SeisSol-coverage
            EXECUTABLE SeisSol-serial-test
            EXCLUDE "/usr/*"
                    "submodules/*"
                    "*/tests/*"
                    "external/*"
                    "*/yaml-cpp-install/*"
    )
  endif()

  set(seissol_test_sources
          src/tests/Model/TestModel.cpp
          src/tests/Initializer/TestInitializer.cpp
          src/tests/Numerical_aux/TestNumerical_aux.cpp
          src/tests/Geometry/TestGeometry.cpp
          src/tests/Kernel/TestKernel.cpp
          src/tests/SourceTerm/TestSourceTerm.cpp
          src/tests/Pipeline/TestPipeline.cpp
          src/tests/ResultWriter/TestResultWriter.cpp
          src/tests/Solver/time_stepping/TestSolverTimeStepping.cpp
          src/tests/DynamicRupture/TestDynamicRupture.cpp
          src/tests/Common/TestCommon.cpp
          )


  if (TESTING_GENERATED)
    set(seissol_test_sources
            ${seissol_test_sources}
            ${CMAKE_CURRENT_BINARY_DIR}/src/generated_code/test-kernel.cpp
            )
  endif()

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Initializer/time_stepping/mesh.h5
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/mesh.h5
    COPYONLY
    )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Initializer/time_stepping/material.yaml
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/material.yaml
    COPYONLY
    )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/ResultWriter/receiver_correct.dat
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/receiver_correct.dat
    COPYONLY
  )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Reader/fsrm_source1.dat
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/fsrm_source1.dat
    COPYONLY
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Reader/fsrm_source2.dat
    ${CMAKE_CURRENT_BINARY_DIR}/Testing/fsrm_source2.dat
    COPYONLY
  )

  if (NETCDF)
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Reader/source_loh.nrf
            ${CMAKE_CURRENT_BINARY_DIR}/Testing/source_loh.nrf
            COPYONLY
    )
    set(seissol_test_sources ${seissol_test_sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/Reader/ReaderTest.cpp)
  endif()

  add_executable(SeisSol-serial-test
            ${seissol_test_sources}
            src/tests/test_main.cpp)
  target_link_libraries(SeisSol-serial-test PRIVATE SeisSol-lib)
if(WITH_GPU)
  target_link_libraries(SeisSol-serial-test PRIVATE general-sycl-offloading)
endif()
  target_include_directories(SeisSol-serial-test PUBLIC external/)
  doctest_discover_tests(SeisSol-serial-test)

  # Avoid duplicate definition of FLOP counters
  target_compile_definitions(SeisSol-serial-test PRIVATE YATETO_TESTING_NO_FLOP_COUNTER)
endif()

install(TARGETS SeisSol-bin RUNTIME)

if (WITH_GPU)
  string(TOUPPER "${DEVICE_BACKEND}" BACKEND_UPPER_CASE)
  set(RUN_GPU_TIME_SELECTOR "${BACKEND_UPPER_CASE}_VISIBLE_DEVICES")
  configure_file(${PROJECT_SOURCE_DIR}/cmake/gpu-launch-template.in
                 ${PROJECT_BINARY_DIR}/seissol-launch
                 @ONLY)
  file(CHMOD ${PROJECT_BINARY_DIR}/seissol-launch PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
  install(FILES ${PROJECT_BINARY_DIR}/seissol-launch DESTINATION ${CMAKE_INSTALL_PREFIX}/share
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
endif()

# https://blog.kitware.com/static-checks-with-cmake-cdash-iwyu-clang-tidy-lwyu-cpplint-and-cppcheck/
# https://ortogonal.github.io/cmake-clang-tidy/
