*****************************************************************************
* UCR DMFB Synthesis Framework                                              *
* http://www.microfluidics.cs.ucr.edu/dmfb_static_simulator/benchmarks.html *
*****************************************************************************

/////////////////////////////////////////////////////////////////////////////
// DAG_to_SCHED File Format (order is unimportant)
/////////////////////////////////////////////////////////////////////////////
DagName (name)
Node (id, DISPENSE, fluidName, volume, name) 
Node (id, MIX, numDropsBefore, time (s), name)
Node (id, DILUTE, numDropsBefore, time (s), name)
Node (id, SPLIT, numDropsAfter, time (s), name)
Node (id, HEAT, time (s), name)
Node (id, DETECT, numDropsIo, time (s), name)
Node (id, OUTPUT, sinkName, name)
Node (id, STORAGE, name)
Node (id, GENERAL, name)
Edge (parentId, childId)
