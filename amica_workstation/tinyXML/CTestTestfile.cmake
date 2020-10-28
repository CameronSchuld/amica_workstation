# CMake generated Testfile for 
# Source directory: C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master
# Build directory: C:/lib/tinyXML
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(xmltest "C:/lib/tinyXML/Debug/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  WORKING_DIRECTORY "C:/lib/tinyXML/Debug" _BACKTRACE_TRIPLES "C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;105;add_test;C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(xmltest "C:/lib/tinyXML/Release/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  WORKING_DIRECTORY "C:/lib/tinyXML/Release" _BACKTRACE_TRIPLES "C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;105;add_test;C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(xmltest "C:/lib/tinyXML/MinSizeRel/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  WORKING_DIRECTORY "C:/lib/tinyXML/MinSizeRel" _BACKTRACE_TRIPLES "C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;105;add_test;C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(xmltest "C:/lib/tinyXML/RelWithDebInfo/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  WORKING_DIRECTORY "C:/lib/tinyXML/RelWithDebInfo" _BACKTRACE_TRIPLES "C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;105;add_test;C:/Users/camer/Downloads/tinyxml2-master/tinyxml2-master/CMakeLists.txt;0;")
else()
  add_test(xmltest NOT_AVAILABLE)
endif()
