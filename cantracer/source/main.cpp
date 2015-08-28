#include <cantools/parsers/dbc_parser.h>
#include <cantools/devices/can_socket.hpp>

#include <fstream>
#include <iostream>

#include <chrono>

#include <linux/can.h>
#include <signal.h>

namespace
{
    devices::can_socket can_socket;
    bool running = true;
}

int main(int argc, char* argv[])
{
    std::fstream dbc_file_to_parse("test.dbc");
    std::stringstream dbc_stream;
    dbc_stream << dbc_file_to_parse.rdbuf();
    auto dbc_file_tree = dbc::parse_dbc(std::move(dbc_stream));

    can_socket.init("can0", CAN_RAW);

    signal(SIGINT, [&](int signum)
    {
        running = false;
        can_socket.close_socket();
        std::cout << "End Triggerblock\n";
        exit(signum);
    });

    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << "date " << std::ctime(&start_time);
    std::cout << "base hex timestamps absolute\n";
    std::cout << "// CAN channel: " << 1 << "\n";
    std::cout << "Begin Triggerblock\n";
    std::cout << "// ;    time  can ident           attr dlc data ... \n";

    while(running)
    {
        can::frame test;
        can_socket.read_frame(test);
        std::chrono::duration< double > elapsed_seconds = std::chrono::system_clock::now() - start;
        std::cout << "\t" << elapsed_seconds.count()
                  << " 1 "
                  << std::hex << test.id
                  << std::dec << " Rx d "
                  << static_cast< unsigned int >(test.dlc)
                  << std::hex;

        for(int i = 0; i < test.dlc; ++i)
        {
            std::cout << " " << static_cast< unsigned int >(test.data[i]);
        }
        std::cout << std::dec << "\n";
    }

    return 0;
}
