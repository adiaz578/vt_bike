This is the BIKE round 2 submission package, which is organized as follows:

Supporting_Documentation:

	BIKE-Spec-Round2.pdf - BIKE round 2 specification.
	BIKE-Changes-Round2.pdf - Sumamry of changes for round 2.

	statement-submitter_Rafael_Misoczki.pdf
	statement-submitter-Olivier_Blazy.pdf
	statement-submitter_Paulo_Barreto.pdf
	statement-submitter-Carlos_Aguilar_Melchor.pdf      
	statement-submitter-Philippe_Gaborit.pdf
	statement-submitter_Nicolas_Sendrier.pdf
	statement-submitter_Edoardo_Persichetti.pdf         
	statement-submitter-Gilles_Zemor.pdf                
	statement-submitter_Shay_Gueron.pdf
	statement-submitter-Jean_Christophe_Deneuville.pdf  
	statement-submitter-Slim_Bettaieb.pdf
	statement-submitter_Jean_Pierre_Tillich.pdf         
	statement-submitter_Tim_Guneysu.pdf
	statement-submitter-Loic_Bidoux.pdf
	statement-submitter-Nicolas_Aragon.pdf
	statement-submitter_Valentin_Vasseur.pdf
	statement-patent-Carlos_Aguilar_Melchor.pdf 
	statement-patent-cnrs.pdf                           
	statement-patent-Philippe_Gaborit.pdf               
	statement-implementation_Nir_Drucker.pdf                 
	statement-implementation-Slim_Bettaieb.pdf          
	statement-implementation_Sridhar_Intel.pdf          
	statement-implementation_Rafael Misoczki.pdf             
	statement-implementation_Shay_Gueron.pdf                 


KAT:
	INDCPA:
		BIKE1: KAT's for BIKE1.
		BIKE2: KAT's for BIKE2.
		BIKE3: KAT's for BIKE3.
	INDCCA:
		BIKE1-CCA: KAT's for BIKE1-CCA.
		BIKE2-CCA: KAT's for BIKE2-CCA.
		BIKE3-CCA: KAT's for BIKE3-CCA.


Reference_Implementation & Optimized_Implementation:
  - README          - Describe the various compilation options available.
  - aes_ctr_prf.c/h - The seed expander API and code.
  - api.h           - As requested by NIST submission.
  - decode.c/h      - The decoding function.
  - defs.h          - Code default definitions.
  - kem.c/h         - Main KEM interface functions.
  - measurements.h  - Methods to measure the code (in cycles/seconds).
  - openssl_utils   - This implementation uses OpenSSL. This file contains 
                      OpenSSL utilities such as BIGNUM conversion,
					  multiplication etc...
  - parallel_hash   - MultiBlock hash.
  - sampling.c/h    - Utilities for sampling pseudorandom data. 
  - types.h         - Datatypes used in this code package.
  - utilities.c/h   - Basic utilities such as print/compare etc...
  - tests (directory) - Files 
    - fixed_seed_test.c 
  - FromNIST (directory) - Files downloaded from NIST website.
    - PQCgenKAT_kem.c
	- rng.c/h

Additional_Implementation:
	See the BIKE-BIKE_c99/README.md file

Hardware_Implementation:
	HDL: The HDL files for the different hardware designs.
	python: The pytho scripts to test the hardware modules.

