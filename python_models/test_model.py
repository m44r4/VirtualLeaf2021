class TestModel:
    def model_id(self):
        return "Test Model From Python"

    def n_chem(self):
        return 6
    
    def cell_housekeeping(self, cell):
        return cell.cell_type()
        # print(cell.cell_type())