[Mesh]
  [init_domain]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 1000
    ny = 1
    xmin = 0
    xmax = 100.0
    ymin = 0.0
    ymax = 0.1
  []
[]

[Variables]
  [temperature]
    initial_condition = 278.15  
    family = LAGRANGE
    order = FIRST
    outputs = none
  []
  [porePressure]
    initial_condition = 10.0
    family = LAGRANGE
    order = FIRST
  []
[]

[Kernels]
  [thermalfluxTerm4]
    type = ADDiffusion
    variable = temperature
  []
  [massTimederiv]
    type = FRADMassTimeDeriv
    variable = porePressure
  []
  [massDarcyKernel]
    type = FRADMassDarcy
    variable = porePressure
    gravityVector = '0.0 0.0 0.0'
  []
[]

[Materials]
  [saturatedSoil]
    type = FRMatGen
    PoreWaterPressure = porePressure
    Temperature = temperature
    
    gravityVector = '0.0 0.0 0.0'
    lambda_form = '1.0 0 0 0 1.0 0 0 0 1.0'
    permeabilitySat = '0.5 0 0 0 0.5 0 0 0 0.5'       

    betaCompressibility = 1.0  
    gammaW = 1.0 
    k_r_option = 3  # Constant option
    k_r_min = 1.0
    porosity = 0.5

    #Unused directly in this simulation, but must be set to non-zero values.
    rho_i = 920.0
    rho_s = 2650.0
    rho_w = 1000.0
    c_i = 2060.0
    c_s = 835.0
    c_w = 4182.0
    lambda_s = 9.0   
    lambda_w = 0.6   
    lambda_i = 2.14   
    lambda_option = 4  
    T_freeze = 269.15
    SFCC_option = 2     
    alpha = 1.06e-06
    beta = 2.62
    gamma = 4.67
    Si_max = 0.95
    W_param = 0.5
    L_f = 334000.0
    T_0 = 273.15
    impedance_factor = 50.0  
    m_hyd = 0.87 
    kici_option = 1
    theta_option = 1
  []
[]

[BCs]
  [leftT]
    type = DirichletBC
    variable = temperature
    boundary = 'left'
    value = 278.15 # +5 C
  []
  [rigthT]
    type = DirichletBC
    variable = temperature
    boundary = 'right'
    value = 278.15 # +5 C
  []
  [leftP]
    type = DirichletBC
    variable = porePressure
    boundary = 'left'
    value = 1000.0
  []
  [rightP]
    type = DirichletBC
    variable = porePressure
    boundary = 'right'
    value = 10.0
  []
[]


[Preconditioning]
  active = 'original'
  [original]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-ksp_type -pc_type -sub_pc_type -sub_pc_factor_shift_type'
    petsc_options_value = 'gmres asm lu NONZERO'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 1.0
  end_time = 10.0
  nl_abs_tol = 1e-6
[]

[Outputs]
  execute_on = 'INITIAL timestep_end'
  exodus = true
  print_linear_residuals = false
  time_step_interval = 1
[]

