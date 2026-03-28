cmake_minimum_required(VERSION 3.8)
project(cashier_system)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)

  # the following line skips the linter which checks for copyrights

  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)

  # the following line skips cpplint (only works in a git repo)

  # comment the line when this package is in a git repo and when

  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

endif()



rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Bill.msg"
  "srv/GetStatus.srv"
)

ament_export_dependencies(rosidl_default_runtime)

#used for code compilation
add_executable(bill_generator src/bill_generator.cpp)

#link ROS libraries
ament_target_dependencies(bill_generator rclcpp)

#use install to make it runnable
install(TARGETS
  bill_generator
  DESTINATION lib/${PROJECT_NAME}
)

#link msg headers
rosidl_target_interfaces(bill_generator ${PROJECT_NAME}  "rosidl_typesupport_cpp")

#inventory_node stuff
add_executable(inventory_node src/inventory_node.cpp)
ament_target_dependencies(inventory_node rclcpp)

rosidl_target_interfaces(inventory_node  ${PROJECT_NAME}
"rosidl_typesupport_cpp")

#status_node stuff

add_executable(status_node src/status_node.cpp)
ament_target_dependencies(status_node rclcpp)

rosidl_target_interfaces(status_node
${PROJECT_NAME} "rosidl_typesupport_cpp")

#inventory_node install stuff
install(TARGETS
bill_generator
inventory_node
status_node
DESTINATION lib/${PROJECT_NAME}
)


ament_package()
