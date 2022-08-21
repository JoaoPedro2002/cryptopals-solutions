from set2.exercise9 import exercise9

class Execute:

    def __init__(self):
        self.__sets = {
            1: self.set1, 2: self.set2, 3: self.set3, 4: self.set4,
            5: self.set5, 6: self.set6, 7: self.set7, 8: self.set8
        }

    def especific_set(self, chosen_set: int):
        return self.__sets[chosen_set]()

    def execute_pattern(self, pattern: list):
        for i in range(len(pattern)):
            self.__sets[i]()
        return self

    def all(self):
        self.set1()\
            .set2()\
            .set3()\
            .set4()\
            .set5()\
            .set5()\
            .set6()\
            .set7()\
            .set8()

    def set1(self):
        exercise9()
        return self

    def set2(self):

        return self

    def set3(self):
        return self

    def set4(self):
        return self

    def set5(self):
        return self

    def set6(self):
        return self

    def set7(self):
        return self

    def set8(self):
        return self
