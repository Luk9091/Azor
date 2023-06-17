from dataclasses import dataclass
import numpy as np

@dataclass
class geometry:
    x       : float = 0
    y       : float = 0
    width   : float = 200
    height  : float = 200

    @property
    def radius(self) -> float:
        if self.width >= self.height:
            return np.sqrt((np.power(2*self.height, 2)+np.power(2*self.height, 2))/2)/2
        elif self.height > self.width:
            return np.sqrt((np.power(2*self.width, 2)+np.power(2*self.width, 2))/2)/2
        

    @radius.setter
    def radius(self, value: float) -> None:
        self.height = np.sqrt(np.power(value, 2)*2)/2
        self.width = self.height
