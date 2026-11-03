# Interfrost T1: 1D freezing of saturated soil with phase change.
# 10 m long 1D domain initially at 4 C, left end held at -6 C, right end at 4 C, 2 days.
# See Grenier et al. (2018), McKenzie et al. (2007) and the InterFrost project (test case T1).

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 500
  ny = 1
  xmin = 0
  xmax = 10.0
  ymin = 0.0
  ymax = 0.1
[]

[Variables]
  [temperature]
    initial_condition = 277.15
    family = LAGRANGE
    order = FIRST
  []
  [porePressure]
    initial_condition = 0.0
    family = LAGRANGE
    order = FIRST
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
  [thermalPhaseChangeTerm]
    type = FRADTempPhaseChange
    variable = temperature
    temperature = temperature
    pressure = porePressure
    L_f = 334560
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

    rho_i = 920.0
    rho_s = 2530.0
    rho_w = 1000.0

    # The reference uses a single volumetric heat capacity c_eff = 690030 J/m^3/K.
    # The phase specific heats are set to 690030 / rho_phase so that
    # rho*c of each phase, and hence the mixture, equals that value.
    c_i = 750.032
    c_s = 272.739
    c_w = 690.030

    # Thermal conductivity: linear interpolation in temperature between the
    # unfrozen value (lambda_w) at T_0 and the frozen value (lambda_i) at T_freeze
    lambda_option = 3
    lambda_s = 2.3    # unused for lambda_option = 3
    lambda_w = 2.41   # unfrozen soil
    lambda_i = 3.46   # frozen soil
    T_freeze = 269.15 # -4 C

    # SFCC: van Genuchten model fitted over 0 to -6 C to the reference bilinear
    # curve (Sw falling linearly from 1 at 0 C to its residual 0.391 at -4 C)
    SFCC_option = 1
    alpha = 1.50e-07
    beta = 1.28
    gamma = 2.13
    Si_max = 0.85
    W_param = 2.3 # unused for SFCC_option = 1

    porosity = 0.336

    betaCompressibility = 1.0e-08

    L_f = 334560

    gammaW = 1.0 # not used in this test, must be nonzero

    T_0 = 273.15

    kici_option = 2
    theta_option = 2

    k_r_option = 1
    k_r_min = 1.0e-6
    impedance_factor = 50
    m_hyd = 0.87
  []
[]

[BCs]
  [leftT]
    type = DirichletBC
    variable = temperature
    boundary = 'left'
    value = 267.15
  []
  [rightT]
    type = DirichletBC
    variable = temperature
    boundary = 'right'
    value = 277.15
  []
  [leftP]
    type = DirichletBC
    variable = porePressure
    boundary = 'left'
    value = 0.0
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
    petsc_options_value = 'hypre       boomeramg'
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  dt = 1200.0
  end_time = 172800.0
  nl_abs_tol = 1e-6
[]

[VectorPostprocessors]
  [temperatureVecPost]
    type = LineValueSampler
    variable = temperature
    start_point = '0.0  0.0 0.0'
    end_point   = '10.0 0.0 0.0'
    num_points = 200
    sort_by = 'x'
    outputs = 'csv2'
  []
[]

[Postprocessors]
  [zeroT_position]
    type = FindValueOnLine
    start_point = '0 0 0'
    end_point = '10 0 0'
    v = 'temperature'
    target = 273.15
    tol = 1e-5
    outputs = 'csv1'
  []
[]

[Outputs]
  execute_on = 'INITIAL timestep_end'
  print_linear_residuals = false
  [csv1]
    type = CSV
  []
  [csv2]
    type = CSV
    sync_only = true
    execute_on = 'timestep_end'
    sync_times = '8640 17280 46200 86400 172800' # 0.1, 0.2, 0.53, 1 and 2 days
    time_data = true
  []
[]
