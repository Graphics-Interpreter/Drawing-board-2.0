SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
SET(EXECUTABLE lab)
ADD_EXECUTABLE(${EXECUTABLE}
	Interface.cpp
	Line.cpp
	Fill.cpp
)
TARGET_LINK_LIBRARIES(${EXECUTABLE} sfml-graphics sfml-window sfml-system)