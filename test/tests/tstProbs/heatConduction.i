[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 10
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 1.0
[]

[Variables]
  [temperature]
    family = LAGRANGE
    order = FIRST
  []
  [porePressure]
    initial_condition = 0.0
    family = LAGRANGE
    order = FIRST
    outputs = none
  []
[]

[ICs]
  [u_ic]
    type = FunctionIC
    variable = 'temperature'
    function = initial_Temperature
  []
[]

[Functions]
  [initial_Temperature]
    type = ParsedFunction
    expression    = '000.00 + 00.0 + 1.0*sin(pi*x)' # Left=0,Middle=1,Right=0
  []
[]

[Kernels]
  [thermaltimederivTerm1]
    type = FRADTempTimeDeriv
    variable = temperature
  []
  [thermalfluxTerm4]
    type = FRADTempConduction
    variable = temperature
  []
  [darcyKernelTerm2]
    type = ADDiffusion
    variable = porePressure
  []
[]

[Materials]
  [saturatedSoil]
    type = FRMatGen
    PoreWaterPressure = porePressure
    Temperature = temperature
    
    gravityVector = '0.0 0.0 0.0'
    lambda_form = '1.0 0 0 0 1.0 0 0 0 1.0'
    permeabilitySat = '1.0 0 0 0 1.0 0 0 0 1.0'

    rho_i = 1
    rho_s = 1
    rho_w = 1

    c_i = 1
    c_s = 1
    c_w = 1

    lambda_i = 0.1
    lambda_s = 0.1
    lambda_w = 0.1
    lambda_option = 1
    
    
    #Unused parameters in this simulation, but must be set to non-zero values.
    m_hyd = 1.0
    alpha = 1.0
    beta = 1.0
    gamma = 1.0
    Si_max = 1.0
    T_freeze = 269.15
    L_f = 334000.0
    T_0 = 273.15
    gammaW = 9810.0
    porosity = 0.4
    SFCC_option = 1
    W_param = 0.5
    k_r_option = 1
    k_r_min = 1e-6
    impedance_factor = 50.0
    betaCompressibility = 1e-8
    kici_option = 1
    theta_option = 1



  []
[]

[BCs]
  [leftT]
    type = DirichletBC
    variable = temperature
    boundary = left
    value = 0 
  []
  [rightT]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 0 
  []
  [topBottomP]
    type = DirichletBC
    variable = porePressure
    boundary = 'left right bottom top'
    value = 0
  []
[]

[Preconditioning]
  [andy]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'hypre       boomeramg'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 0.01
  end_time = 0.1
  nl_abs_tol = 1e-6
[]

[Outputs]
  execute_on = 'INITIAL timestep_end'
  exodus = true
  print_linear_residuals = false
  time_step_interval = 1
[]

