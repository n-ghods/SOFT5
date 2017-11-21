function par = ReactionParams
    par.Nads = @{soft.model.Nads};
    par.Ncomp = @{soft.model.Ncomp};
    par.Ncomp_T = @{soft.model.Ncomp_T};
    par.Nrx_g = @{soft.model.Nrx_g};
    par.Nrx_s = @{soft.model.Nrx_s};

    par.Ac = @{soft.model.Ac};
    par.ADENT = @{arraySpan(soft.model.ADENT)};
    par.B = @{arraySpan(soft.model.B)};
    par.BET_a0 = @{soft.model.BET_a0};
    par.CP = @{table(soft.model.CP, 4, 6)};
    par.CP_s = @{soft.model.CP_s};
    par.deltaHf = @{arraySpan(soft.model.deltaHf)}';
    par.deltaHf_s = @{arraySpan(soft.model.deltaHf_s)}';
    par.Dt = @{soft.model.Dt};
    par.E = @{arraySpan(soft.model.E)};
    par.Ea_s = @{arraySpan(soft.model.Ea_s)};
    par.ENT298 = @{arraySpan(soft.model.ENT298)};
    par.F_sol_w = @{soft.model.F_sol_w};
    par.Flowin_w = @{soft.model.Flowin_w};
    par.Fluxin_w = @{soft.model.Fluxin_w};
    par.FRACin = @{arraySpan(soft.model.FRACin)};
    par.FRACin_s = @{arraySpan(soft.model.FRACin_s)};
    par.Fw = @{arraySpan(soft.model.Fw)};
    par.Gs0 = @{soft.model.Gs0};
    par.Heq = @{arraySpan(soft.model.Heq)};
    par.K_0i = @{arraySpan(soft.model.K_0i)};
    par.k_0j = @{arraySpan(soft.model.k_0j)};
    par.k0_s = @{arraySpan(soft.model.k0_s)};
    par.Keq_j = @{arraySpan(soft.model.Keq_j)};
    par.MW = @{arraySpan(soft.model.MW)};
    par.MW_O2 = @{soft.model.MW_O2};
    par.MW_s = @{arraySpan(soft.model.MW_s)};
    par.n_rx = @{arraySpan(soft.model.n_rx)};

    par.Pin = @{soft.model.Pin};
    par.R0_NiAl2O4 = @{soft.model.R0_NiAl2O4};
    par.R0_NiO = @{soft.model.R0_NiO};
    par.rhogm = @{soft.model.rhogm};
    par.rhogmWin = @{soft.model.rhogmWin};
    par.RHOin = @{soft.model.RHOin};
    par.rhos0 = @{soft.model.rhos0};
    par.Smu = @{arraySpan(soft.model.Smu)};
    par.Stoich = @{table(soft.model.Stoich, 6, 4)};
    par.Stoich_s = @{table(soft.model.Stoich_s, 9, 4)};
    par.Tin = @{soft.model.Tin};
    par.Uin = @{soft.model.Uin};
    par.Yin = @{arraySpan(soft.model.Yin)};

    par.Param.g = @{soft.model.g};
    par.Param.R = @{soft.model.R};
    par.Param.C1 = @{soft.model.C1};
    par.Param.C2 = @{soft.model.C2};
    par.Param.A0 = @{soft.model.A0};
    par.Param.dg = @{soft.model.dg};
    par.Param.dp = @{soft.model.dp};
    par.Param.mug = @{soft.model.mug};
    par.Param.rhog = @{soft.model.rhog};

    par.membrane.P0 = @{soft.model.P0};
    par.membrane.Ea = @{soft.model.Ea};
    par.membrane.tm = @{soft.model.tm};
    par.membrane.dm = @{soft.model.dm};
    par.membrane.Nm = @{soft.model.Nm};

    par.Param.memb.P0 = @{soft.model.P0};
    par.Param.memb.Ea = @{soft.model.Ea};
    par.Param.memb.tm = @{soft.model.tm};
    par.Param.memb.dm = @{soft.model.dm};
    par.Param.memb.Nm = @{soft.model.Nm};
end
