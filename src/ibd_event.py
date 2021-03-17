import ROOT
from ROOT import *
import numpy as np
from particle import *
from constants import *


class IBD_event:

    def __init__(self, E_nu, nu_dir, e_angle_generator):
        self.nuebar = particle(NUEBAR, 0, E_nu, nu_dir)
        self.ebar = particle(EBAR, e0_e, 0, np.array([0, 0, 0]))
        self.n = particle(NEUTRON, e0_n, 0, np.array([0, 0, 0]))

        ROOT.gRandom.SetSeed(0)
        self.rng = ROOT.TRandom3()
        cos_e_theta = e_angle_generator(E_nu)
        self.initialize(cos_e_theta)
        #print(self.nuebar.dir)

    
    def initialize(self, cos_e_theta):
        '''
        Calculate everything about the event from the positron angle. 
        Azimuthal angle is randomly generated
        '''
        self.fillElectronDirAndEnergy(cos_e_theta)
        self.fillNeutronDirAndEnergy()



    def fillElectronDirAndEnergy(self, cos_e_theta):
        # from https://github.com/itscubist/newton/blob/master/src/event.cc
        initialTargetMass = e0_p
        A = -2 * e0_p + 2 * self.nuebar.en*(cos_e_theta - 1)
        B = 2*initialTargetMass*self.nuebar.en + pow(initialTargetMass,2)- pow(e0_n,2)-pow(e0_e,2)
        delta = sqrt(pow(B,2)+4*pow(self.ebar.m,2)*A)
        eMinus = (-B - delta)/(2*A)
        self.ebar.en = eMinus if eMinus > e0_e else e0_e

        # set direction
        phi = self.rng.Uniform(2*np.pi)
        # direction treating neutrino direction as (0, 0, 1)
        direction = spherical_to_cartesian(1, np.arccos(cos_e_theta), phi)
        # get rotation based on neutrino direction
        self.ebar.dir = combine_directions(self.nuebar.dir, direction)

    
    def fillNeutronDirAndEnergy(self):
        self.n.en = self.nuebar.en + e0_p - self.ebar.en
        #print(self.nuebar.en)
        #print(e0_p)
        #print(self.ebar.en)
        #print(self.n.getMomentum())
        p = [0., 0., 0.]
        for i in range(3):
            p[i] = self.nuebar.dir[i] * self.nuebar.getMomentum() - self.ebar.dir[i]*self.ebar.getMomentum()
        #print(np.linalg.norm(p))
        self.n.dir = normalize(p)




    
