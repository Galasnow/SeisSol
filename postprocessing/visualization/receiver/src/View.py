##
# @file
# This file is part of SeisSol.
#
# @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
#
# @section LICENSE
# Copyright (c) 2015, SeisSol Group
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# @section DESCRIPTION
#

from PyQt4.QtGui import *
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
import matplotlib.pyplot as plt

import math
import Navigation
import numpy
import scipy.fftpack

class View(QWidget):

  def __init__(self, parent = None):
    super(View, self).__init__(parent)

    self.figure = plt.figure()
    self.canvas = FigureCanvas(self.figure)
    self.canvas.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)    
    toolbar = NavigationToolbar(self.canvas, self)
    
    self.navigationLayout = QHBoxLayout()
    
    layout = QHBoxLayout(self)
    self.navigations = []
    for i in range(0, 2):
      self.addNavigation()
    
    addIcon = QIcon.fromTheme('list-add')
    addNaviButton = QPushButton(addIcon, 'Add navigation', self)
    addNaviButton.clicked.connect(self.addNavigation)
    
    self.maxFreq = QDoubleSpinBox(self)
    self.maxFreq.setValue(10.0)
    self.maxFreq.setVisible(False)
    spectrumIcon = QIcon.fromTheme('network-wireless')
    self.spectrum = QPushButton(spectrumIcon, 'Spectrum', self)
    self.spectrum.setCheckable(True)
    self.spectrum.clicked.connect(self.plot)
    self.spectrum.toggled.connect(self.maxFreq.setVisible)
    self.maxFreq.valueChanged.connect(self.plot)

    toolLayout = QHBoxLayout()
    toolLayout.addWidget(addNaviButton)
    toolLayout.addWidget(self.spectrum)
    toolLayout.addWidget(self.maxFreq)
    toolLayout.addWidget(toolbar)
    plotLayout = QVBoxLayout()
    plotLayout.addLayout(toolLayout)
    plotLayout.addWidget(self.canvas)
    layout.addLayout(self.navigationLayout)
    layout.addLayout(plotLayout)
    
  def addNavigation(self):
    navigation = Navigation.Navigation(self)
    navigation.activeItemChanged.connect(self.plot)
    navigation.setSizePolicy(QSizePolicy.Maximum, QSizePolicy.Minimum)
    self.navigationLayout.addWidget(navigation)
    self.navigations.append(navigation)

  def plot(self):
    waveforms = []
    numPlots = 0
    names = set()
    for nav in self.navigations:
      for wf in nav.getActiveWaveforms():
        numPlots = max(numPlots, len(wf.names))
        names.update(set(wf.names))
        waveforms.append(wf)
        
    self.figure.clear()
    if numPlots > 0:
      names = list(names)
      names.sort()

      numRows = math.ceil(math.sqrt(numPlots));
      numCols = math.ceil(numPlots / numRows)
      subplots = dict()
      for i in range(0, len(names)):
        subplots[ names[i] ] = self.figure.add_subplot(numRows, numCols, i+1)

      for wf in waveforms:
        for name in wf.names:
          p = subplots[name]
          if self.spectrum.isChecked():
            n = len(wf.waveforms[name])
            dt = wf.time[1]-wf.time[0] # assume equally spaced samples
            f = scipy.fftpack.fftfreq(n, dt)
            W = dt * scipy.fftpack.fft(wf.waveforms[name])
            maxFreqIndices = numpy.argwhere(f > self.maxFreq.value())
            L = maxFreqIndices[0] if len(maxFreqIndices) > 0 else n/2
            p.semilogy(f[1:L], numpy.absolute(W[1:L]))
            p.set_xlabel('f [Hz]')
          else:
            p.plot(wf.time, wf.waveforms[name])
            p.set_xlabel('t (s)')
          p.set_ylabel(name)

      self.figure.tight_layout()
    self.canvas.draw()
