class Grid:
    GRID_SIZE = 9
    BOX_SIZE = 3

    def __init__(self):
        self.grid = [[0] * self.GRID_SIZE for _ in range(self.GRID_SIZE)]

    def get_row(self, row):
        return self.grid[row]

    def get_column(self, col):
        return [self.grid[i][col] for i in range(self.GRID_SIZE)]

    def get_box(self, row, col):
        box = []
        box_row_start = (row // self.BOX_SIZE) * self.BOX_SIZE
        box_col_start = (col // self.BOX_SIZE) * self.BOX_SIZE
        for i in range(self.BOX_SIZE):
            for j in range(self.BOX_SIZE):
                box.append(self.grid[box_row_start + i][box_col_start + j])
        return box

    def clone(self):
        new_grid = Grid()
        new_grid.grid = [row[:] for row in self.grid]
        return new_grid

    def serialize(self):
        return ''.join(str(num) for row in self.grid for num in row)

    @staticmethod
    def deserialize(data):
        if len(data) != Grid.GRID_SIZE * Grid.GRID_SIZE:
            raise ValueError("Invalid serialized data length.")
        new_grid = Grid()
        for i in range(Grid.GRID_SIZE):
            for j in range(Grid.GRID_SIZE):
                new_grid.grid[i][j] = int(data[i * Grid.GRID_SIZE + j])
        return new_grid

    def __eq__(self, other):
        return self.grid == other.grid


class Sudoku(Grid):
    def parse(self, input):
        if len(input) != self.GRID_SIZE * self.GRID_SIZE:
            raise ValueError("Input must be a string of length 81.")
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                self.grid[i][j] = int(input[i * self.GRID_SIZE + j])

    def get_inference(self):
        candidates = [set() for _ in range(self.GRID_SIZE * self.GRID_SIZE)]
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                if self.grid[i][j] == 0:
                    used_numbers = set()
                    row = self.get_row(i)
                    col = self.get_column(j)
                    box = self.get_box(i, j)

                    used_numbers.update(row)
                    used_numbers.update(col)
                    used_numbers.update(box)

                    for num in range(1, 10):
                        if num not in used_numbers:
                            candidates[i * self.GRID_SIZE + j].add(num)
        return candidates


def test_sudoku(input, expected_serialization, expected_inference):
    sudoku = Sudoku()
    try:
        sudoku.parse(input)
        serialized = sudoku.serialize()
        print("Serialized:", serialized)
        print("Expected:", expected_serialization)
        assert serialized == expected_serialization, "Serialization Test failed!"
        print("Serialization Test passed!")

        candidates = sudoku.get_inference()
        for i in range(Grid.GRID_SIZE * Grid.GRID_SIZE):
            assert candidates[i] == expected_inference[i] or (not candidates[i] and expected_inference[i] == {}), f"Inference Test for cell {i} failed!"
            print(f"Inference Test for cell {i} passed!")

    except Exception as e:
        print(e)


if __name__ == "__main__":
    test_sudoku(
        "017903600000080000900000507072010430000402070064370250701000065000030000005601720",
        "017903600000080000900000507072010430000402070064370250701000065000030000005601720",
        [{2, 4, 5, 8},{},{},{},{2, 4, 5},{},{},{4, 8},{2, 4, 8},{2, 3, 4, 5, 6},{2, 3, 4, 5},{3, 6},{1, 2, 5, 7},{},{4, 5, 6, 7},{1, 3, 9},{1, 4, 9},{1, 2, 3, 4, 9},{},{2, 3, 4, 8},{3, 6, 8},{1, 2},{2, 4, 6},{4, 6},{},{1, 4, 8},{},{5, 8},{},{},{5, 8},{},{5, 6, 8, 9},{},{},{6, 8, 9},{1, 3, 5, 8},{3, 5, 8, 9},{3, 8, 9},{},{5, 6, 9},{},{1, 8, 9},{},{1, 6, 8, 9},{1, 8},{},{},{},{},{8, 9},{},{},{1, 8, 9},{},{2, 3, 4, 8, 9},{},{2, 8},{2, 4, 9},{4, 8, 9},{3, 8, 9},{},{},{2, 4, 6, 8},{2, 4, 8, 9},{6, 8, 9},{2, 5, 7, 8},{},{4, 5, 7, 8, 9},{1, 8, 9},{1, 4, 8, 9},{1, 4, 8, 9},{3, 4, 8},{3, 4, 8, 9},{},{},{4, 9},{},{},{},{3, 4, 8, 9}]
    )
