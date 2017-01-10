from sklearn import tree
import csv
import matplotlib.pyplot as plt

class DecisionTree:
    def __init__(self, filename):
        self.X_train = []
        self.Y_train = []
        self.X_test = []
        self.Y_test = []
        self.train_errors = []
        self.test_errors = []
        
        self.min_samples_leaves = [i for i in range(1,26)]
        self.max_depths = [i for i in range(2, 21)]
        self.load_file(filename)
    
    def load_file(self, filename):
        file_obj = open(filename)
        count = 0
        for line in file_obj.readlines():
            data = line.strip().split(',')
            x = map(float, data[2:])
            y = data[1]
            if count < 400:
                self.X_train.append(x)
                self.Y_train.append(y)
            else:
                self.X_test.append(x)
                self.Y_test.append(y)
            count +=1
    
    def execute_min_leaves(self):
        self.train_errors = []
        self.test_errors = []
        for min_samples_leaf in self.min_samples_leaves:
            #initialize our tree model
            clf = tree.DecisionTreeClassifier(criterion = 'gini',
                                              min_samples_leaf = min_samples_leaf)
            self.execute(clf)
        
        self.draw_plot('min_samples_leaves.png', 'Plot of Error vs. min_samples_leaf',
                       self.min_samples_leaves, 'min_samples_leaf')


    def execute_max_depth(self):
        self.train_errors = []
        self.test_errors = []
        for max_depth in self.max_depths:
            #initialize our tree model
            clf = tree.DecisionTreeClassifier(criterion = 'gini',
                                              max_depth = max_depth)
            self.execute(clf)
    
        self.draw_plot('max_depths.png', 'Plot of Error vs. max_depths',
                       self.max_depths, 'max_depth')

    def execute(self, clf):
        #train the model
        clf = clf.fit(self.X_train, self.Y_train)
        
        #make prediction
        G_train = clf.predict(self.X_train)
        G_test = clf.predict(self.Y_test)
        
        #compute error
        train_error = self.compute_error(G_train, self.Y_train)
        test_error = self.compute_error(G_test, self.Y_test)
        self.train_errors.append(train_error)
        self.test_errors.append(test_error)
    
    def compute_error(self, G, Y):
        error = 0
        for i in range(len(G)):
            if G[i] != Y[i]:
                error +=1
            return 1.0 * error /len(G)

    def draw_plot(self, filename, title, x_data, x_label):
        plt.clf()
        plt.plot(x_data, self.train_errors)
        plt.plot(x_data, self.test_errors)
        plt.xlabel(x_label)
        plt.ylabel('error')
        plt.title(title)
        plt.legend(['train_error', 'test_error'])
        
        #save file
        save_dir = '/Users/LeiyaMa/Desktop/' + filename
        plt.savefig(save_dir, bbox_inches='tight')

if __name__ == '__main__':
    #change the saving path if not found
    dtree = DecisionTree('/Users/LeiyaMa/Desktop/wdbc_change.txt')
    dtree.execute_min_leaves()
    dtree.execute_max_depths()