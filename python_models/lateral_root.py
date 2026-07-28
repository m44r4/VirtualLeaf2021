import virtualleaf
print("loaded lateral_root from:", __file__)
print("virtualleaf module object:", virtualleaf)
 
class Par:
    def __init__(self, cell_expansion_rate,rel_cell_div_threshold):
        self.rel_cell_div_threshold=rel_cell_div_threshold
        self.cell_expansion_rate=cell_expansion_rate
    

class lateral_root:
    def model_id(self):
        return "Lateral Root from python"
    
    def n_chem(self):
        return 1
    
    def on_divide (self, parent_info, daughter1,daughter2):
        if daughter1.cell_type() == 3:
            if daughter1.centroid().x < daughter2.centroid().x:
                daughter1.set_chemical(0,0.49)
                daughter2.set_chemical(0,1.0)
            else:
                daughter2.set_chemical(0,0.49)
                daughter1.set_chemical(0,1.0)
        print(daughter1.cell_type() , "daughter 1 cell type")

    
    def cell_housekeeping(self, cell):
        par = Par(1.0,2.0)
        if cell.cell_type() == 3:
            if cell.chemical(0) > 0.5 or cell.chemical(0) < 0.001:
                cell.enlarge_target_area(par.cell_expansion_rate)
                if cell.area() > par.rel_cell_div_threshold * virtualleaf.base_area():
                    print(virtualleaf.base_area())
                    cell.divide()
                    

 