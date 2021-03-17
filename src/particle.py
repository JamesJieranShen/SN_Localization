import ROOT
from ROOT import *
import numpy as np
from constants import *


def normalize(vector):
        vector = np.array(vector)
        norm = np.linalg.norm(vector)
        #print(norm)
        return vector/norm if norm != 0 else vector

def spherical_to_cartesian(r, theta, phi):
        x = r*np.sin(theta)*np.cos(phi)
        y = r*np.sin(theta)*np.sin(phi)
        z = r*np.cos(theta)
        return np.array([x, y, z])




def combine_directions(v1, v2):
    '''
    In the basis where v1 is (0, 0, 1), the vector's representation is v2
    Return v2 in the original basis

    Explaination: Consider rotational matrix R
    R*zhat = nuebar_dir
    R*xhat and R*yhat should belong in nuebar_dir's null space
    R is the concatenation of two null space vectors, and nuebar_dir itself.
    '''

    # find two vectors orthogonal to nuebar_dir
    xhat = np.array([1., 0., 0.])
    if v1[0] == 0:  # perpendicular to xhat, so use it as one orthogonal vector
        n1 = xhat
    else:
        n1 = normalize(np.cross(v1, xhat))
    
    n2 = normalize(np.cross(v1, n1))
    R = np.array([n1, n2, v1])
    return np.matmul(R, v2)



class particle:
    def __init__(self, id, mass, energy, direction):
        self.id = id
        self.m = mass
        self.en = energy
        self.dir = normalize(direction)
        #self.p = np.sqrt(self.en**2 - self.m**2)
    
    def getMomentum(self):
        return sqrt(self.en**2 - self.m**2)