
[ -d $HOME/arm-sve-tools ] || cp -a ~ri-jlinford/arm-sve-tools $HOME

module use /lustre/software/aarch64/tools/arm-compiler/20.3/modulefiles
module use /lustre/projects/bristol/modules-a64fx/modulefiles
module load Generic-SVE/RHEL/8/arm-linux-compiler-20.3/armpl/20.3.0
export ARM_LICENSE_DIR=/lustre/software/aarch64/tools/arm-compiler/licences

