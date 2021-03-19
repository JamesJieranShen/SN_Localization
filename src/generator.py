# This file performs the basic functions explained/tested in Generator.ipynb. It is here so that it can be referenced by other scripts.

import ROOT
from ROOT import *
import numpy as np
from scipy.spatial.transform import Rotation
import ctypes

from constants import *
from particle import *
from ibd_event import *


class Generator:
    
    xscn = TH2F("ibd_double_diff_xscn", "IBD Double Differential Cross Section", 200, 0.5, 200.5, 20, -1, 1)
    rng = ROOT.TRandom3()
    
    def __init__(self, xscn_path):
        xscn_file = np.loadtxt(xscn_path, delimiter=" ")

        for line in xscn_file:
            energy = line[0]
            for i in range(1, line.size):
                angle = -0.95 + 0.1*(i-1) # center of the bin
                self.xscn.Fill(energy, angle, line[i])
    
    def get_e_angle(self, nu_energy):
        nbin = self.xscn.GetXaxis().FindBin(nu_energy)
        py = self.xscn.ProjectionY("projection_y", nbin, nbin)
        ret = py.GetRandom()
        py.Delete()
        return ret
    
    
    def generate(self, nu_energy, nuebar_dir=np.array([0, 0, 1])):
        event = IBD_event(nu_energy, nuebar_dir, self.get_e_angle)
        return event
        
        

    
