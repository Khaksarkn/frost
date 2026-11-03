# Field TH simulation under a seasonal surface temperature. Restarts from the
# smoothed initial temperature field in fieldCase_init.e.
#
# For localized surface heating, activate the associated boundary condition
# (fireTconv) and its control (fire_on).

ZKelvin = 273.15
DaySec = 86400.0

fire_start = '${fparse DaySec * 5}'
fire_peak =  '${fparse DaySec * 6}'
fire_end =   '${fparse DaySec * 15}'

fire_temp_start =   '${fparse ZKelvin + 20.0}'
fire_temp_peak =    '${fparse ZKelvin + 100.0}'
fire_temp_end =     '${fparse ZKelvin + 100.0}'

[Mesh]
  [input_file]
    type = FileMeshGenerator
    file = fieldCase_init.e
    use_for_exodus_restart = true
  []
  [leftTopSurf]
    type = ParsedGenerateSideset
    combinatorial_geometry = 'x<=-5.00 & x>=-15.00 & y >= -0.01 & y <= 0.01'
    new_sideset_name = 'leftTopSurfBoundary'
    input = 'input_file'
  []
  [midTopSurf]
    type = ParsedGenerateSideset
    combinatorial_geometry = 'x<=5.00 & x>=-5.00 & y >= -0.01 & y <= 0.01'
    new_sideset_name = 'midTopSurfBoundary'
    input = 'leftTopSurf'
  []
  [rightTopSurf]
    type = ParsedGenerateSideset
    combinatorial_geometry = 'x<=15.00 & x>=5.00 & y >= -0.01 & y <= 0.01'
    new_sideset_name = 'rightTopSurfBoundary'
    input = 'midTopSurf'
  []
[]

[Variables]
  [temperature]
    initial_from_file_var = temperature
    initial_from_file_timestep = 1
    family = LAGRANGE
    order = FIRST
    scaling = 1e-2
  []
  [porePressure]
    family = LAGRANGE
    order = FIRST
    scaling = 1e7
  []
[]

[ICs]
  [pwp_hydrostatic]
    type = FunctionIC
    variable = porePressure
    function = pwp_hydrostaic_linear
  []
[]

[Controls]
  inactive = 'fire_on'
  [fire_on]
    type = ConditionalFunctionEnableControl
    enable_objects = 'BCs::fireTconv'
    conditional_function = fireFunc
    implicit = false
    execute_on = 'initial timestep_begin'
  []
[]

[Functions]
  [pwp_hydrostaic_linear]
    type = ParsedFunction
    expression = -y*gammaW
    symbol_names = 'gammaW'
    symbol_values = '9810'
  []

  # Surface air temperature history over one year (s, K)
  [Temp_history]
    type = PiecewiseLinear
    x = '0.       6073404   10949595  16442178  19931353  27237542  31886633'
    y = '272.55   290.82    290.24    272.62    258.39    258.17    271.57'
  []

  # Environmental temperature during the heating event
  [fireTemp]
    type = PiecewiseLinear
    x = '${fire_start}       ${fire_peak}         ${fire_end}'
    y = '${fire_temp_start}   ${fire_temp_peak}    ${fire_temp_end}'
  []

  # 1 while the heating event is active, 0 otherwise
  [fireFunc]
    type = ParsedFunction
    expression = 'if(t >= ${fire_start} & t < ${fire_end}, 1, 0)'
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
    gravityVector = '0.0 1.0 0.0'
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

    T_0 = 273.15
    permeabilitySat = '1.0e-8 0 0 0 1.0e-8 0 0 0 1.0e-8'
    gravityVector = '0 1.0 0'
    lambda_form = '1.0 0 0 0 1.0 0 0 0 1.0'
    porosity = 0.43
    rho_s = 2620
    rho_i = 917
    rho_w = 1000

    # Thermal conductivity: volume-weighted mixture of soil, water and ice (option 4)
    lambda_option = 4
    lambda_i = 2.24
    lambda_s = 1.96
    lambda_w = 0.56
    T_freeze = 269.15 # unused for lambda_option = 4

    c_i = 2000
    c_s = 900
    c_w = 4180
    L_f = 334000

    # SFCC: van Genuchten model
    SFCC_option = 1
    alpha = 1.0e-6
    beta = 2.1
    gamma = 0.7
    W_param = 0.5 # unused for SFCC_option = 1

    # Relative permeability: bounded Mualem-van Genuchten model
    k_r_option = 2
    k_r_min = 1e-6
    impedance_factor = 50.0 # unused for k_r_option = 2
    m_hyd = 0.7

    betaCompressibility = 1e-8

    Si_max = 0.89

    gammaW = 9810.0

    kici_option = 2
    theta_option = 2
  []
[]

[BCs]
  inactive = 'fireTconv'
  [fireTconv]
    type = FRFunctionEnvTempCoupledNeumannBC
    variable = temperature
    boundary = 'midTopSurfBoundary'
    temperature = temperature
    temperatureE = fireTemp
    lambdaE = 5.0
  []
  [topTconv]
    type = FRFunctionEnvTempCoupledNeumannBC
    variable = temperature
    boundary = 'leftTopSurfBoundary midTopSurfBoundary rightTopSurfBoundary'
    temperature = temperature
    temperatureE = Temp_history
    lambdaE = 2.68
  []
  [bottomT]
    type = DirichletBC
    variable = temperature
    boundary = bottom
    value = 271.15
  []

  [topP]
    type = DirichletBC
    variable = porePressure
    boundary = top
    value = 0
  []
[]

[Preconditioning]
  [andy]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -snes_atol '
    petsc_options_value = 'lu 1e-05'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 1200
  nl_abs_tol = 1e-5
  end_time = 31886633
[]

[Outputs]
  [exo]
    type = Exodus
    execute_on = 'timestep_end'
    sync_only = true
    sync_times = '${fparse DaySec * 30} ${fparse DaySec * 360}'
    show = 'temperature'
  []
[]
