[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 400
  ny = 1
  xmin = -10.0
  xmax = 10.0
  ymin = 0.0
  ymax = 0.1
[]

[Variables]
  [temperature]
    family = LAGRANGE
    order = FIRST
  []
  [porePressure]
    family = LAGRANGE
    order = FIRST
    outputs = none
  []
[]

[ICs]
  [T_ic]
    type = FunctionIC
    variable = 'temperature'
    function = initial_Temperature
  []
  [P_ic]
    type = FunctionIC
    variable = 'porePressure'
    function = initial_pressure
  []
[]

[Functions]
  [initial_Temperature]
    type = ParsedFunction
    expression = 'if(x<=0.0, 294.15, 293.15)' # Left=21C, Right=20C
  []
  [initial_pressure]
    type = ParsedFunction
    expression  = '10.0 - 10.0*x' # Linear, left=0.001, right=0
  []
[]

[Kernels]
# ---HEAT
  [thermaltimederivTerm1]
    type = FRADTempTimeDeriv
    variable = temperature
  []
  [thermalfluxTerm4]
    type = FRADTempConduction
    variable = temperature
  []
  [thermalAdvectionTerm2]
    type = FRADTempAdvection
    variable = temperature
  []
# ---MASS
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

    rho_i = 1.0
    rho_s = 1.0
    rho_w = 1.0

    c_i = 1.0
    c_s = 1.0
    c_w = 1.0

    lambda_i = 0.1
    lambda_s = 0.1
    lambda_w = 0.1
    lambda_option = 1
    
    gammaW = 1.0 
    m_hyd = 1.0
    
    #Unused parameters in this simulation, but must be set to non-zero values.
    alpha = 1.0
    beta = 1.0
    gamma = 1.0
    Si_max = 1.0
    T_freeze = 269.15
    L_f = 334000.0
    T_0 = 273.15
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
    value = 294.15 # 21 C
  []
  [rightT]
    type = DirichletBC
    variable = temperature
    boundary = right
    value = 293.15 # 20 C
  []
  [leftP]
    type = DirichletBC
    variable = porePressure
    boundary = 'left'
    value = 20.0
  []
  [rightP]
    type = DirichletBC
    variable = porePressure
    boundary = 'right'
    value = 0.0
  []
[]

[Preconditioning]
  [andy]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_hypre_type'
    petsc_options_value = 'lu       boomeramg'
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

