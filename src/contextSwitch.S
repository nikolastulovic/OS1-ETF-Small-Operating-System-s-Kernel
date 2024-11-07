.global _ZN6Kernel6Thread13contextSwitchEPNS0_7ContextES2_
.type _ZN6Kernel6Thread13contextSwitchEPNS0_7ContextES2_, @function

_ZN6Kernel6Thread13contextSwitchEPNS0_7ContextES2_:
    #a0 - old
    #a1 - new context
    sd ra, 0*8(a0)
    sd sp, 1*8(a0)

    ld ra, 0*8(a1)
    ld sp, 1*8(a1)

    ret