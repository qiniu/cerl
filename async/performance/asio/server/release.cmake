#使用方法
#./debug.sh

#项目名称 
#debug版本
PROJECT(server)

#头文件的目录
include_directories(/usr/include /usr/local/include ../../../../../async/include ../../../../../venus/include ../../../../../stdext/include)   

#库文件的目录
link_directories(/usr/lib /usr/local/lib )  


#当前目录src目录，存放源代码
AUX_SOURCE_DIRECTORY(../ DIR_SRCS)

#逐个添加源程序
#LIST(APPEND DIR_SRCS ../../net/message_queue.h)  

#排除win main
LIST(REMOVE_ITEM DIR_SRCS ../win_main.cpp)

set(CMAKE_CXX_FLAGS "-g0 -O2")
#可执行文件名称
ADD_EXECUTABLE( server.exe ${DIR_SRCS})

#连接用的库名称	
target_link_libraries( server.exe boost_system boost_thread pthread )


add_custom_target( cover
	COMMAND lcov --directory .  --capture --output-file app.info
  COMMAND genhtml  -o  results  app.info 
	COMMAND firefox results/index.html &

	)

add_custom_target(check
  COMMAND  valgrind --leak-check=full --show-reachable=yes ./server.exe
 
)
