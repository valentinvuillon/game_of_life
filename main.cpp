#include <SFML/Graphics.hpp>
#include <iostream>
#include "random.hpp"
#include <string>


using Random = effolkronium::random_static;

class line {
public:
	std::array<sf::Vertex, 2> vertices;

	line(int x1, int y1, int x2, int y2, sf::Color color) {
		vertices[0] = sf::Vertex{ sf::Vector2f(x1, y1), color };
		vertices[1] = sf::Vertex{ sf::Vector2f(x2, y2), color };
	}

	void set_color(sf::Color color) {
		vertices[0].color = color;
		vertices[1].color = color;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Lines);
	}

};

class line_group {
public:
	std::vector<line*> group;

	line_group(int nb_lines, int nb_columns, int cell_size, sf::Color color) {
		for (int i = 0; i < nb_columns + 1; i++) {
			this->add_line(new line(i * cell_size, 0, i * cell_size, nb_lines * cell_size, color));
		}

		for (int i = 0; i < nb_lines; i++) {
			this->add_line(new line(0, i * cell_size, nb_columns * cell_size, i * cell_size, color));
		}
	}

	void add_line(line* line) {
		group.push_back(line);
	}

	void set_color(sf::Color color) {
		for (int i = 0; i < group.size();i++) {
			group[i]->set_color(color);
		}
	}

	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < group.size();i++) {
			group[i]->draw(window);
		}
	}

};

class game_of_life{
	public:
	int** current_state;
	int** next_state;
	int nb_rows, nb_columns;

	int cell_size;


	game_of_life(int nb_rows_, int nb_columns_, int cell_size_){
		nb_columns = nb_columns_;
		nb_rows =  nb_rows_;
		cell_size = cell_size_;

		current_state = new int*[nb_rows];
		for (int i = 0; i < nb_rows; i++) {
			current_state[i] = new int[nb_columns]();
		}

		for(int i = 0; i < nb_rows; i++){
			for(int j=0; j < nb_columns; j++){
				current_state[i][j] = 0;

			}
		}

		next_state = new int*[nb_rows];
		for (int i = 0; i < nb_rows; i++) {
			next_state[i] = new int[nb_columns]();
		}
	}

	void copy_state(int** state1, int** state2) {
		for (int i = 0; i < nb_rows; i++) {
			for (int j = 0; j < nb_columns; j++) {
				state1[i][j] = state2[i][j];
			}
		}
	}

	void draw(sf::RenderWindow& window){
		for(int i = 0; i < nb_rows; i++){
			for(int j=0; j < nb_columns; j++){
				if(current_state[i][j] == 1){
					sf::RectangleShape square(sf::Vector2f(cell_size, cell_size));
					square.setPosition(sf::Vector2f(cell_size * j, cell_size * i));
					window.draw(square);

				}
			}
		}
	}

	int check_validity_row(int row){
		if(row == nb_rows){
			return 0;
		}

		if(row == -1){
			return nb_rows -1;
		}

		return row;
	}

	int check_validity_col(int col){
		if(col == nb_columns){
			return 0;
		}

		if(col == -1){
			return nb_columns -1;
		}

		return col;
	}

	int check_nb_neighbors(int i, int j){
		int pos_vec[8][2] = {{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
		int neighbors_count = 0;

		for(auto elt:pos_vec){

			int row = check_validity_row(i + elt[1]);
			int col = check_validity_col(j + elt[0]);

			if(current_state[row][col] == 1){
				neighbors_count += 1;
			}
		}

		return neighbors_count;
	}

	void update(){
		copy_state(next_state, current_state);

		for(int i = 0; i < nb_rows; i++){
			for(int j = 0; j < nb_columns; j++){
				if(current_state[i][j] == 1){
					int nb_neighbors = check_nb_neighbors(i, j);

					if(nb_neighbors < 2 || nb_neighbors >3){
						next_state[i][j] = 0;
					}


				}else{
					if(check_nb_neighbors(i, j) == 3){
						next_state[i][j] = 1;
					}
				}
			}
		}

		copy_state(current_state, next_state);

	}

	void switch_cell_state(int row, int column){
		int state = current_state[row][column];
		current_state[row][column] = 1 - state;
	}

	void clear(){
		for (int i = 0; i < nb_rows; i++) {
			for (int j = 0; j < nb_columns; j++) {
				current_state[i][j] = 0;
			}
		}
	}

	void random(){
		for(int i = 0; i < nb_rows; i++){
			for(int j=0; j < nb_columns; j++){
				if(Random::get(0, 100)>90){
					current_state[i][j] = 1;
				}else{
					current_state[i][j] = 0;
				}
			}
		}
	}

};

void screen_coord_to_cell_coord(int screen_coord_x, int screen_coord_y, int& row, int& column, unsigned int cell_size){
	row = (int)screen_coord_y / cell_size;
	column = (int)screen_coord_x / cell_size;
}

class button{
	public:
	unsigned int x_coord;
	unsigned int y_coord;
	unsigned int width;
	unsigned int height;
	sf::Text text;

	button(unsigned int x_coord_, unsigned int y_coord_, unsigned int width_, unsigned int height_, std::string str, sf::Font& font, sf::Color color, int text_size): text(font){
		x_coord = x_coord_;
		y_coord = y_coord_;
		width = width_;
		height = height_;

		text.setString(str);
		text.setCharacterSize(text_size);
		text.setFillColor(color);
		text.setPosition(sf::Vector2f(x_coord + 5, y_coord ));
	}

	void draw(sf::RenderWindow& window)
	{
		sf::RectangleShape rectangle(sf::Vector2f(width, height));
		rectangle.setPosition(sf::Vector2f(x_coord, y_coord));
		window.draw(rectangle);

		window.draw(text);
	}

	bool pressed(unsigned int pos_x, unsigned int pos_y)
	{
		if(pos_x > x_coord && pos_x < x_coord + width && pos_y > y_coord && pos_y < y_coord + height)
		{
			return true;
		}else{
			return false;
		}
	}
	
};


int main()
{

	unsigned int nb_rows = 40;
	unsigned int nb_columns = 65;

	unsigned int cell_size = 20;
	unsigned int right_panel_width = 150;

	unsigned int width = nb_columns * cell_size + right_panel_width; 
	unsigned int height = nb_rows * cell_size;
    
	sf::RenderWindow window(sf::VideoMode({ width, height }), "game of life");
	window.setFramerateLimit(20);

	line_group grid(nb_rows, nb_columns, cell_size, sf::Color(255, 255, 255, 50));
	game_of_life gol(nb_rows, nb_columns, cell_size);
	
	bool paused = true;

	sf::Font font("Arial.ttf");
	button pause_button(nb_columns * cell_size +5, 5, right_panel_width -10, 2 * cell_size, "play/pause", font, sf::Color::Black, 25);
	button clear_button(nb_columns * cell_size +5, 10 + 2 * cell_size, right_panel_width -10, 2 * cell_size, "clear", font, sf::Color::Black, 25);
	button random_button(nb_columns * cell_size +5, 15 + 4 * cell_size, right_panel_width -10, 2 * cell_size, "random init", font, sf::Color::Black, 25);


	while(window.isOpen()){

		while (const std::optional event = window.pollEvent()){
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					int pos_x = mouseButtonPressed->position.x;
					int pos_y = mouseButtonPressed->position.y;

					if(clear_button.pressed(pos_x, pos_y)){
						gol.clear();
					}

					if(pause_button.pressed(pos_x, pos_y)){
						paused = !paused;
					}

					if(random_button.pressed(pos_x, pos_y)){
						gol.random();
					}

					if( pos_x < nb_columns * cell_size){
						int row;
						int column;
						screen_coord_to_cell_coord(pos_x , pos_y, row, column, cell_size);
						gol.switch_cell_state(row, column);
					}

				}
			}

		}

		window.clear(sf::Color::Black);

		grid.draw(window);
		gol.draw(window);
		pause_button.draw(window);
		clear_button.draw(window);
		random_button.draw(window);

		window.display();

		if(!paused){
			gol.update();
		}

	}
		

    return 0;

}

