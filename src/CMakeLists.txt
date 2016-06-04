SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
SET(EXECUTABLE lab)
ADD_EXECUTABLE(${EXECUTABLE}
	lab.cpp
	Line.cpp
)
TARGET_LINK_LIBRARIES(${EXECUTABLE} sfml-graphics sfml-window sfml-system)