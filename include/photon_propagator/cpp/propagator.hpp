#include <map>
#include <deque>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

#include <curand_kernel.h>

#include <photon_propagator/device.hpp>
#include <photon_propagator/configuration.hpp>
#include <photon_propagator/ice_model.hpp>
#include <photon_propagator/geometry.hpp>
#include <photon_propagator/optical_module_lines.hpp>
#include <photon_propagator/hits.hpp>
#include <photon_propagator/photons.hpp>

class Propagator{
private:
  const std::shared_ptr<Device>& device_; 
  const Configuration& configuration_;
  const IceModel& ice_model_;
  const Geometry& geometry_;
  const OpticalModuleLines& om_lines_;
     
  unsigned int photons_per_launch_;         // number of photons per launch
  unsigned int photon_objects_per_launch_;  // number of photons per launch
  unsigned number_of_blocks_;               // number of blocks
  int threads_per_block_;                   // number of threads per block
        
  Hits host_hits_;

  curandState_t* __device_rng_state;
  
  // the following four members are just for profiling and diagnostics
  float time_spent_on_device_;
  cudaStream_t stream_;
  cudaEvent_t event1_;
  cudaEvent_t event2_;

  void allocate_curand_memory();
  
public:
  
  Propagator(const std::shared_ptr<Device>& device,
	    const Configuration& configuration,
	    const IceModel& ice_model,
	    const Geometry& geometry,
	    const OpticalModuleLines& om_lines,
	    const size_t hit_buffer_size,
	    const size_t photon_size);
	      
  ~Propagator();

  // this just propagates photons.
  // the conversion from particles to photons
  // happens elsewhere.
  void propagte(const Photons& host_photons);
};

