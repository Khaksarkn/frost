# Interfrost TH2, no hydraulic gradient: thawing of a frozen inclusion.
# The mesh is generated here; no external files are needed.
# See Grenier et al. (2018) and the InterFrost project (test case TH2).

delt_t = 600.0
op_start = 11e4
op_end = 13e4
t_final = 200000.0

leftSidePressure = 0.0

[Mesh]
  [init_domain]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 360
    ny = 60
    xmin = 0
    xmax = 3.0
    ymin = -0.0
    ymax = 0.5
    subdomain_ids = 1
    subdomain_name = 'outside'
  []
  [inclusion]
    type = SubdomainBoundingBoxGenerator
    input = init_domain
    block_id = 2
    block_name = 'inclusion'
    bottom_left = '0.8335 -0.1665 0'
    top_right =   '1.1665  0.1665 0'
  []
[]

[Variables]
  [temperature]
    family = LAGRANGE
    order = FIRST
  []
  [porePressure]
    family = LAGRANGE
    order = FIRST
  []
[]

[ICs]
  [tempIC]
    type = ConstantIC
    value = 278.15
    variable = temperature
    block = 'outside'
  []
  [inclusionIC]
    type = ConstantIC
    value = 268.15
    variable = temperature
    block = 'inclusion'
  []
  [pressureIC]
    type = FunctionIC
    function = initial_porePressure
    variable = porePressure
  []
[]

[Functions]
  [initial_porePressure]
    type = ParsedFunction
    expression = 'p_leftside*(1-x/x_length)'
    symbol_names = 'p_leftside x_length'
    symbol_values = '${leftSidePressure} 3.0'
  []

  [time_step_function]
    type = PiecewiseConstant
    x = '0                  ${fparse op_start}   ${fparse op_end}   ${fparse t_final}'
    y = '${fparse delt_t/4}   ${fparse delt_t/4}  ${fparse delt_t/4}   ${fparse delt_t/4}'
  []
[]

[AuxVariables]
  [waterContent]
    order = CONSTANT
    family = MONOMIAL
  []
[]

[AuxKernels]
  [waterContentAux]
    type = FRWaterContentAux
    variable = waterContent
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
  [thermalAdvectionTerm2]
    type = FRADTempAdvection
    variable = temperature
  []
  [thermalPhaseChangeTerm]
    type = FRADTempPhaseChange
    variable = temperature
    temperature = temperature
    pressure = porePressure
    L_f = 334000
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
  [massPhaseChangeKernel]
    type = FRADMassPhaseChange
    variable = porePressure
    pressure = porePressure
    temperature = temperature
  []
[]

[Materials]
  [saturatedSoil]
    type = FRMatGen
    PoreWaterPressure = porePressure
    Temperature = temperature

    gravityVector = '0.0 0.0 0.0'
    lambda_form = '1.0 0 0 0 1.0 0 0 0 1.0'
    permeabilitySat = '7.11e-4 0 0 0 7.11e-4 0 0 0 7.11e-4' # m/s; k_int = 1.3e-10 m^2, mu = 1.793e-3 kg/m/s

    rho_i = 920.0
    rho_s = 2650.0
    rho_w = 1000.0

    c_i = 2060.0
    c_s = 835.0
    c_w = 4182.0

    # Thermal conductivity: volume-weighted mixture of soil, water and ice (option 4)
    lambda_option = 4
    lambda_s = 9.0
    lambda_w = 0.6
    lambda_i = 2.14
    T_freeze = 269.15 # unused for lambda_option = 4

    # SFCC: exponential model, Sw = (1 - Sw_res) exp(-(T - T_0)^2 / W^2) + Sw_res
    SFCC_option = 2
    W_param = 0.5
    Si_max = 0.95
    alpha = 1.06e-06 # unused for SFCC_option = 2
    beta = 2.62      # unused for SFCC_option = 2
    gamma = 4.67     # unused for SFCC_option = 2

    porosity = 0.37

    betaCompressibility = 1e-8

    L_f = 334000.0

    gammaW = 9810.0

    T_0 = 273.15

    # Relative permeability: impedance (exponential) model, k_r = max(10^(-Omega n Si), k_r_min)
    k_r_option = 4
    k_r_min = 1e-6
    impedance_factor = 50.0 # Omega
    m_hyd = 0.87 # unused for k_r_option = 4

    kici_option = 2
    theta_option = 2
  []
[]

[BCs]
  [leftT]
    type = DirichletBC
    variable = temperature
    boundary = 'left'
    value = 278.15
  []
  [leftP]
    type = DirichletBC
    variable = porePressure
    boundary = 'left'
    value = ${leftSidePressure}
  []
  [rightP]
    type = DirichletBC
    variable = porePressure
    boundary = 'right'
    value = 0.0
  []
[]

[Preconditioning]
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
  dt = ${delt_t}
  end_time = ${t_final}
  nl_abs_tol = 1e-6

  [TimeStepper]
    type = FunctionDT
    function = time_step_function
  []

  [Adaptivity]
    interval = 1
    refine_fraction = 0.3
    coarsen_fraction = 0.3
    max_h_level = 5
  []
[]

[Postprocessors]
  [min_temperature]
    type = NodalExtremeValue
    variable = temperature
    value_type = min
    outputs = 'csv1'
  []
  # Total unfrozen water volume in the domain
  [functor_matprop]
    type = ADElementIntegralFunctorPostprocessor
    functor = waterContent
    outputs = 'csv1'
  []
[]

[Outputs]
  print_linear_residuals = false

  [exodus_output]
    type = Exodus
    execute_on = 'INITIAL timestep_end'
    sync_only = true
    sync_times = '20000'
    show = 'temperature'
  []
  [csv1]
    type = CSV
    execute_on = 'timestep_end'
  []
  # Forces the time stepper to land exactly on these times
  [csv2]
    type = CSV
    execute_on = 'timestep_end'
    sync_only = true
    sync_times = '8640 17280 46200 86400 172800'
    time_data = true
  []
[]
