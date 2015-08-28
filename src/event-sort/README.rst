==========
EVENT SORT
==========

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Tue Aug 25 2015 $

GitHub repo
-----------

Move to a folder in AFS::

  mathemage@mathemage-TTL-TEKNOPRO:~$ cd /afs/cern.ch/user/k/kha/
  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ mkdir xphi-lhcb/

and clone the created repo on GitHub::

  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ git clone git@github.com:mathemage/xphi-lhcb.git

Instructions
------------

Rainer instructed to simulate event sorting on Xeon Phi.

Comments on the source code
---------------------------

Code has been written for one architecture without offloads: either running entirely on host or entirely on a MIC.

main.cpp
~~~~~~~~

- `mep_factor` = number of collision that are to be currently processed (default: 1000 collisions)
- `iterations` = number of iterations, each time prefix sum and MEP copy is simulated on Xeon Phi
- `total_sources` = number of sources, i.e. 500 cards each with 2 inputs from the detector -> default:1000 sources
- `min_length`, `max_length` = lower and upper for the size of 1 collision data from 1 source
- `sources` = array of arrays of lengths, `sources[i]` = array of random data lengths for source #i

For other functions and variables, see `../prefix-offset/README.rst`.

include/utils.cpp
~~~~~~~~~~~~~~~~~

Following functions has been added:

- `allocate_sources()` = allocate buffers per each source: (`total_sources` x `mep_factor`) random lengths
- `fill_sources_with_random_lengths()` = fill all pre-allocated buffers with random integers in the range [`min_len`, `max_len`]
- `deallocate_sources()` = deallocate memory allocated for buffers by `allocate_sources()`
