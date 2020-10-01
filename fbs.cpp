/**
 * Copyright (C) <2020>  <Yuqian Cao (Kerry)>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { // check cmd args
        std::cout << "ERROR: fbwatch <CCID> <MAX_PROCESS_COUNT>" << std::endl;
        return 0;
    }

    char buf[2048]; // line buffer
    std::string ccid = argv[1]; // user ccid
    int max_p = std::stoi(argv[2], nullptr, 10); // max count for number of process

    for (;;) {
        // get processes that belong to ccid
        FILE *pipe = popen(std::string("ps aux | grep " + ccid).c_str(), "r");
        if (!pipe) {
            std::cerr << "ERROR: Unable to execute ps" << std::endl;
            return 0;
        }

        // count the number of processes that belong to ccid
        int count = 0;
        while (!feof(pipe)) {
            if (fgets(buf, 2048, pipe) != nullptr){
                count += 1;
            }
        }

        pclose(pipe); // wait for child process

        if (count > max_p) {
            pipe = popen(std::string("killall -u " + ccid).c_str(), "r");
            if (!pipe) {
                std::cerr << "WARNING: There is a fork bomb and I couldn't kill it" << std::endl;
                pclose(pipe);
                return 0;
            }
        } else if (count == 0) {
            std::cout << "ERROR: I think you've entered the wrong CCID" << std::endl;
            return 0;
        }

        usleep(200);
    }
}
