% Definisjoner:
% 
% Stoichiometric matrices: One for solid rx (Nrx*Nspec_solid) (CH4 CO H2 H2O) , 1 for gas rx (Nrx_gas*Nspec_gas) (CH4 CO CO2 H2 H2O N2)
% One row per rx
% One column per specie in Phenom order
% 
% Struct:
% eqType for solid
% eqtype for gas
% parameters for eqType 

function par=ReactionParams
	par.eqType_g=3;
	% par.eqType_s=2;
	
    par.Stoich=[ %Ngasspecies*Nrx
	-1     0    -1     0
     1    -1     0     0
     0     1     1     0
     3     1     4    -1
    -1    -1    -2     0
     0     0     0     0
	 ];
	 
	 % par.Stoich_s=[%(Nsolidspec(3)+Ngasspec(6))*Nrx_solid
	% -1     0     0     0
     % 0     0    -1     0
     % 1     0     1     0
     % 0    -1     0     0
     % 2     1     0     0
     % 0     0     0     0
     % 4     1     1     0
    % -4    -1    -1     0
     % 0     0     0     0
	 % ];
	 
% par.??=?? %Whatever eqType_g/s requires
% ...	 
%eqType3dummy: f(T,p,x)=par.A*exp(-par.E/R/T)*(prod(p(par.Ireactants(:)).^par.nreactants(:))-prod(p(par.Iproducts(:)).^par.nproducts(:)));
par.Ireactants=[3 4]; %CO2+H2
par.Iproducts=[2 5]; %CO + H2O
par.nreactants=[1 1];
par.nproducts=[1 1];
 
par.A=12345;
par.E=343454;


end

