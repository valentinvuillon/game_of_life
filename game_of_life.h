


class game_of_life {
public:
	int** current_state;
	int** next_state;
	int rows, cols;
	int cell_size;



	void copy_state(int** state1, int** state2) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				state1[i][j] = state2[i][j];
			}
		}
	}

	game_of_life(int cell_nb_width, int cell_nb_height, int cell_size_) {
		rows = cell_nb_height;
		cols = cell_nb_width;
		cell_size = cell_size_;

		current_state = new int* [rows];
		for (int i = 0; i < rows; i++) {
			current_state[i] = new int[cols]();
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (Random::get(0, 100) > 90) {
					current_state[i][j] = 1;
				}
				else {
					current_state[i][j] = 0;
				}
			}
		}

		next_state = new int* [rows];
		for (int i = 0; i < rows; i++) {
			next_state[i] = new int[cols]();
		}
	}

	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (current_state[i][j] == 1) {
					sf::RectangleShape square(sf::Vector2f(cell_size, cell_size));
					square.setPosition(sf::Vector2f(cell_size * j, cell_size * i));
					window.draw(square);

				}
			}
		}
	}

	int check_validity_row(int row) {
		if (row == rows) {
			return 0;
		}

		if (row == -1) {
			return rows - 1;
		}

		return row;
	}

	int check_validity_col(int col) {
		if (col == cols) {
			return 0;
		}

		if (col == -1) {
			return cols - 1;
		}

		return col;
	}

	int check_nb_neighbors(int i, int j) {
		int pos_vec[8][2] = { {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };
		int neighbors_count = 0;

		for (auto elt : pos_vec) {

			int row = check_validity_row(i + elt[1]);
			int col = check_validity_col(j + elt[0]);

			if (current_state[row][col] == 1) {
				neighbors_count += 1;
			}
		}

		return neighbors_count;
	}

	void update() {
		copy_state(next_state, current_state);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (current_state[i][j] == 1) {
					int nb_neighbors = check_nb_neighbors(i, j);

					if (nb_neighbors < 2 || nb_neighbors >3) {
						next_state[i][j] = 0;
					}


				}
				else {
					if (check_nb_neighbors(i, j) == 3) {
						next_state[i][j] = 1;
					}
				}
			}
		}

		copy_state(current_state, next_state);

	}
};


