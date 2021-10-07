#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>

std::vector<std::vector<int>> matrix;

bool* visited;
bool dfs(int N, int p);

int main(int argc, char** argv) {

    // program can be used with command line argument
    // this argument used for handling .dot source file path
    // if argument list is empty, program create temp.dot file and scan adjacency matrix from cin
    // after generating file will be removed

    std::string source = "";

    if(argc == 2) {

        source = argv[1];

    }

    if(source.length()) { // checking for file exists if program used with command line argument

        std::fstream file;
        file.open(source);

        if(!file.is_open()) {

            std::cout << "File doesn\'t exist, terminating..." << std::endl;
            return 0;

        }

    }

    if(!source.length()) {

        std::cout << "Enter adjacency matrix: " << std::endl;

        std::vector<int> temp; // temp vector for row scanning
        std::string input = "";

        std::getline(std::cin, input); // scanning one row for matrix size calculation

        int num;
        std::stringstream ss(input);

        while(ss >> num)
            temp.push_back(num); // size of temp on this step - num of columns and rows in matrix

        matrix.push_back(temp);
        int matrix_size = temp.size();

        for(int i = 0; i < matrix_size - 1; i++) { // scanning all from cin rows to matrix

            temp.clear();
            input = "";

            std::getline(std::cin, input);

            std::stringstream ss(input);

            while(ss >> num)
                temp.push_back(num);

            matrix.push_back(temp);

        }

        std::string dot_file = "graph my_graph{\n"; // generating temp.dot file content

        for(int i = 0; i < (int)matrix.size(); i++) {

            for(int j = 0; j <= i; j++) {

                if(matrix[i][j]) {

                    dot_file += "\t";
                    dot_file += (i + 1 + '0');
                    dot_file += " -- ";
                    dot_file += (j + 1 + '0');
                    dot_file += " [label=\"";
                    dot_file += matrix[i][j] + '0';
                    dot_file += "\",label=\"";
                    dot_file += matrix[i][j] + '0';
                    dot_file += "\"]\n";

                }
            }
        }

        dot_file += "}";

        std::fstream temp_file;

        temp_file.open("temp.dot", std::ios::out);
        temp_file << dot_file; // writing temp.dot to file
        temp_file.close();

    }

    std::string rm_dcommand = "";

    if(!source.length()) { // command for removing temp.dot file, filling string only if command line argument not presented

        rm_dcommand = "rm ";
        rm_dcommand += "temp.dot";

    }

    source = (!source.length()) ? "temp.dot" : source;

    std::string jpeg_fname = source; // name of .jpeg file
    jpeg_fname += ".jpeg";

    std::string gen_command = "dot "; // command for generating .jpeg file
    gen_command += source;
    gen_command += " -Tjpeg > ";
    gen_command += jpeg_fname;

    std::string create_command = "touch "; // command for creating .jpeg file
    create_command += jpeg_fname;

    std::string rm_jcommand = "rm "; // command for removing .jpeg file
    rm_jcommand += jpeg_fname;

    system(create_command.c_str()); // creating jpeg temp image
    system(gen_command.c_str()); // generating jpeg temp image

    visited = (bool*)malloc(sizeof(bool) * matrix.size()); // allocating memory for bool array

    bool cycle_check = false;

    for(int k = 0; k < matrix.size(); k++) { // checking graph for cycles

        for(int i = 0; i < matrix.size(); i++)
            visited[i] = false;

        if(dfs(k, k)) {
            cycle_check = true;
            break;
        }

    }

    if(cycle_check)
        std::cout << "Graph has cycle(s)" << std::endl;
    else
        std::cout << "Graph doesn\'t have cycles" << std::endl;

    sf::Image jpeg;
    jpeg.loadFromFile(jpeg_fname); // loading temp image for getting dimensions

    sf::Texture ready_image;
    ready_image.loadFromFile(jpeg_fname); // loading texture for shape background

    sf::RenderWindow window(sf::VideoMode(jpeg.getSize().x, jpeg.getSize().y), "Graph"); // creating window

    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(jpeg.getSize().x, jpeg.getSize().y));
    shape.setTexture(&ready_image); // creating rectangle shape with texture background

    system(rm_jcommand.c_str()); // removing jpeg temp image

    if(rm_dcommand.length())
        system(rm_dcommand.c_str()); // if program used without command line argument, removing temp.dot file

    while(window.isOpen()) { // drawing window with SFML

        sf::Event event;

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    free(visited);

    return 0;
}

bool dfs(int N, int p){ // deep first search

    visited[N] = true;

    for(int i = 0; i < matrix.size(); i++) {

        if(matrix[N][i] && i != p && !visited[i])
            return dfs(i, N);

        else if(matrix[N][i] && i != p && visited[i])
            return true;

    }

    return false;
}

