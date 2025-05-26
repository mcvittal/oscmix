#include <assert.h>
#include <stddef.h>
#include "device.h"
#include "intpack.h"

#define LEN(a) (sizeof (a) / sizeof *(a))

static const char *const reflevel_input[] = {"+13dBu", "+19dBu"};
static const char *const reflevel_output_xlr[] = {"+4dBu", "+13dBu", "+19dBu", "+24dBu"};
static const char *const reflevel_output[] = {"+4dBu", "+13dBu", "+19dBu"};
static const char *const reflevel_phones[] = {"Low", "High"};

#define GAIN_MIC_MIN 0
#define GAIN_MIC_MAX 750
#define GAIN_INST_MIN 80
#define GAIN_INST_MAX 500

static const struct channelinfo inputs[] = {
	{"Analog 1",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 2",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 3",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 4",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 5",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 6",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 7",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
    {"Analog 8",  INPUT_HAS_GAIN | INPUT_HAS_REFLEVEL, .gain={0, 120}, .reflevel={reflevel_input, LEN(reflevel_input)}},
	// TODO: Change device.h (and probably in oscmix.h/c) to get this working
	/*
	 {"Mic/Inst 9",  INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ,
			.gain={GAIN_MIC_MIN, GAIN_MIC_MAX},
		.inst_gain={GAIN_INST_MIN, GAIN_INST_MAX}},
	{"Mic/Inst 10", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ,
			.gain={GAIN_MIC_MIN, GAIN_MIC_MAX},
		.inst_gain={GAIN_INST_MIN, GAIN_INST_MAX}},
	{"Mic/Inst 11", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ,
			.gain={GAIN_MIC_MIN, GAIN_MIC_MAX},
		.inst_gain={GAIN_INST_MIN, GAIN_INST_MAX}},
	{"Mic/Inst 12", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ,
			.gain={GAIN_MIC_MIN, GAIN_MIC_MAX},
		.inst_gain={GAIN_INST_MIN, GAIN_INST_MAX}},*/
	{"Mic/Inst 9",  INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ, .gain={0, 750}},
	{"Mic/Inst 10", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ, .gain={0, 750}},
	{"Mic/Inst 11", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ, .gain={0, 750}},
	{"Mic/Inst 12", INPUT_HAS_GAIN | INPUT_HAS_48V | INPUT_HAS_AUTOSET | INPUT_HAS_HIZ, .gain={0, 750}},
    // AES
	{"AES L"}, {"AES R"},
    // ADAT
	{"ADAT 1"}, {"ADAT 2"}, {"ADAT 3"}, {"ADAT 4"}, {"ADAT 5"}, {"ADAT 6"}, {"ADAT 7"}, {"ADAT 8"},
	{"ADAT 9"}, {"ADAT 10"}, {"ADAT 11"}, {"ADAT 12"}, {"ADAT 13"}, {"ADAT 14"}, {"ADAT 15"}, {"ADAT 16"},
    // MADI
    {"MADI 1"}, {"MADI 2"}, {"MADI 3"}, {"MADI 4"}, {"MADI 5"}, {"MADI 6"}, {"MADI 7"}, {"MADI 8"},
    {"MADI 9"}, {"MADI 10"}, {"MADI 11"}, {"MADI 12"}, {"MADI 13"}, {"MADI 14"}, {"MADI 15"}, {"MADI 16"},
    {"MADI 17"}, {"MADI 18"}, {"MADI 19"}, {"MADI 20"}, {"MADI 21"}, {"MADI 22"}, {"MADI 23"}, {"MADI 24"},
    {"MADI 25"}, {"MADI 26"}, {"MADI 27"}, {"MADI 28"}, {"MADI 29"}, {"MADI 30"}, {"MADI 31"}, {"MADI 32"},
    {"MADI 33"}, {"MADI 34"}, {"MADI 35"}, {"MADI 36"}, {"MADI 37"}, {"MADI 38"}, {"MADI 39"}, {"MADI 40"},
    {"MADI 41"}, {"MADI 42"}, {"MADI 43"}, {"MADI 44"}, {"MADI 45"}, {"MADI 46"}, {"MADI 47"}, {"MADI 48"},
    {"MADI 49"}, {"MADI 50"}, {"MADI 51"}, {"MADI 52"}, {"MADI 53"}, {"MADI 54"}, {"MADI 55"}, {"MADI 56"},
    {"MADI 57"}, {"MADI 58"}, {"MADI 59"}, {"MADI 60"}, {"MADI 61"}, {"MADI 62"}, {"MADI 63"}, {"MADI 64"}
    // How does Spdif fit in... start of adat2?
};
_Static_assert(LEN(inputs) == 94, "bad inputs");

static const struct channelinfo outputs[] = {
	{"Analog 1", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output_xlr, LEN(reflevel_output_xlr)}},
	{"Analog 2", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output_xlr, LEN(reflevel_output_xlr)}},
	{"Analog 3", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Analog 4", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Analog 5", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Analog 6", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Analog 7", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Analog 8", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_output, LEN(reflevel_output)}},
	{"Phones 9", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_phones, LEN(reflevel_phones)}},
	{"Phones 10", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_phones, LEN(reflevel_phones)}},
	{"Phones 11", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_phones, LEN(reflevel_phones)}},
	{"Phones 12", OUTPUT_HAS_REFLEVEL, .reflevel={reflevel_phones, LEN(reflevel_phones)}},
    // AES
	{"AES L"}, {"AES R"},
    // ADAT
	{"ADAT 1"}, {"ADAT 2"}, {"ADAT 3"}, {"ADAT 4"}, {"ADAT 5"}, {"ADAT 6"}, {"ADAT 7"}, {"ADAT 8"},
	{"ADAT 9"}, {"ADAT 10"}, {"ADAT 11"}, {"ADAT 12"}, {"ADAT 13"}, {"ADAT 14"}, {"ADAT 15"}, {"ADAT 16"},
    // MADI
    {"MADI 1"}, {"MADI 2"}, {"MADI 3"}, {"MADI 4"}, {"MADI 5"}, {"MADI 6"}, {"MADI 7"}, {"MADI 8"},
    {"MADI 9"}, {"MADI 10"}, {"MADI 11"}, {"MADI 12"}, {"MADI 13"}, {"MADI 14"}, {"MADI 15"}, {"MADI 16"},
    {"MADI 17"}, {"MADI 18"}, {"MADI 19"}, {"MADI 20"}, {"MADI 21"}, {"MADI 22"}, {"MADI 23"}, {"MADI 24"},
    {"MADI 25"}, {"MADI 26"}, {"MADI 27"}, {"MADI 28"}, {"MADI 29"}, {"MADI 30"}, {"MADI 31"}, {"MADI 32"},
    {"MADI 33"}, {"MADI 34"}, {"MADI 35"}, {"MADI 36"}, {"MADI 37"}, {"MADI 38"}, {"MADI 39"}, {"MADI 40"},
    {"MADI 41"}, {"MADI 42"}, {"MADI 43"}, {"MADI 44"}, {"MADI 45"}, {"MADI 46"}, {"MADI 47"}, {"MADI 48"},
    {"MADI 49"}, {"MADI 50"}, {"MADI 51"}, {"MADI 52"}, {"MADI 53"}, {"MADI 54"}, {"MADI 55"}, {"MADI 56"},
    {"MADI 57"}, {"MADI 58"}, {"MADI 59"}, {"MADI 60"}, {"MADI 61"}, {"MADI 62"}, {"MADI 63"}, {"MADI 64"}
};
_Static_assert(LEN(outputs) == 94, "bad outputs");

#define N_CHAN_REGS 0x30

static enum control eq_dyn_reg_to_ctl(int const reg) {
    switch (reg) {
        case 0x00: return LOWCUT;
        case 0x01: return LOWCUT_FREQ;
        case 0x02: return LOWCUT_SLOPE;
        case 0x03: return EQ;
        case 0x04: return EQ_BAND1TYPE;
        case 0x05: return EQ_BAND1GAIN;
        case 0x06: return EQ_BAND1FREQ;
        case 0x07: return EQ_BAND1Q;
        case 0x08: return EQ_BAND2GAIN;
        case 0x09: return EQ_BAND2FREQ;
        case 0x0A: return EQ_BAND2Q;
        case 0x0B: return EQ_BAND3TYPE;
        case 0x0C: return EQ_BAND3GAIN;
        case 0x0D: return EQ_BAND3FREQ;
        case 0x0E: return EQ_BAND3Q;
        case 0x0F: return DYNAMICS;
        case 0x10: return DYNAMICS_GAIN;
        case 0x11: return DYNAMICS_ATTACK;
        case 0x12: return DYNAMICS_RELEASE;
        case 0x13: return DYNAMICS_COMPTHRES;
        case 0x14: return DYNAMICS_COMPRATIO;
        case 0x15: return DYNAMICS_EXPTHRES;
        case 0x16: return DYNAMICS_EXPRATIO;
        case 0x17: return AUTOLEVEL;
        case 0x18: return AUTOLEVEL_MAXGAIN;
        case 0x19: return AUTOLEVEL_HEADROOM;
        case 0x1A: return AUTOLEVEL_RISETIME;
        default: return UNKNOWN;
    }
}

#define DYN_METER_BASE 0x3264
#define AUTO_LEVEL_BASE 0x3328
#define ROOM_EQ_BASE 0x3427

#define MIX_REGION_SIZE (LEN(inputs) * 2 * 64)  // Inputs, playbacks 64 output slots

static enum control
regtoctl(int reg, struct param *p)
{
	int idx;  // Do we need this?

	if (reg < 0)
		return -1;
    if (reg < LEN(inputs) * N_CHAN_REGS) {
        idx = reg / N_CHAN_REGS;
        reg -= N_CHAN_REGS * idx;
        p->in = idx;
        switch (reg) {
            case 0x00: return INPUT_MUTE;
            case 0x01: return INPUT_FXSEND;
            case 0x02: return INPUT_STEREO;
            case 0x03: return INPUT_RECORD;
            case 0x04: return UNKNOWN;
            case 0x05: return INPUT_PLAYCHAN;
            // case 0x06: return INPUT_WIDTH;  // TODO: This doesn't exist on UCXII
            case 0x07: return INPUT_MSPROC;
            case 0x08: return INPUT_PHASE;
            case 0x09: return INPUT_GAIN;
            case 0x0A: return (inputs[idx].flags & INPUT_HAS_48V) ? INPUT_48V : INPUT_REFLEVEL;
            case 0x0B: return INPUT_HIZ;
            case 0x0C: return INPUT_AUTOSET;
            default: return eq_dyn_reg_to_ctl(reg - 0x0D);
        }
    } else if (reg < (LEN(inputs) + LEN(outputs)) * N_CHAN_REGS) {
        idx = (reg / N_CHAN_REGS) - LEN(inputs);
        reg -= N_CHAN_REGS * idx;
        p->out = idx;
        switch (reg) {
            case 0x00: return OUTPUT_VOLUME;
            case 0x01: return OUTPUT_PAN;
            case 0x02: return OUTPUT_MUTE;
            case 0x03: return OUTPUT_FXRETURN;
            case 0x04: return OUTPUT_STEREO;
            case 0x05: return OUTPUT_RECORD;
            case 0x06: return OUTPUT_PLAYCHAN;
            case 0x07: return OUTPUT_PHASE;
            case 0x08: return OUTPUT_REFLEVEL;
            case 0x09: return OUTPUT_CROSSFEED;
            case 0x0A: return UNKNOWN;
            case 0x0B: return OUTPUT_VOLUMECAL;
            default: return eq_dyn_reg_to_ctl(reg - 0x0C);
        }
    } else if (reg < 0x3000) {
        idx = (reg - 0x2340) / 0x0A;
        reg -= 0x2340 + 0x0A * idx;
        switch (reg) {
            /* TODO: PLAYBACK stuff must work somehow?
            case 0x0: return PLAYBACK_MUTE;
            case 0x1: return PLAYBACK_FXSEND;
            case 0x2: return PLAYBACK_STEREO;
            case 0x3: return PLAYBACK_WIDTH;
            case 0x4: return PLAYBACK_MSPROC;
            case 0x5: return PLAYBACK_PHASE;
            */
            default: return UNKNOWN;
        }
    } else if (reg < 0x4000) {
        switch (reg) {
            case 0x3000: return REVERB;
            case 0x3001: return REVERB_TYPE;
            case 0x3002: return REVERB_PREDELAY;
            case 0x3003: return REVERB_LOWCUT;
            case 0x3004: return REVERB_ROOMSCALE;
            case 0x3005: return REVERB_ATTACK;
            case 0x3006: return REVERB_HOLD;
            case 0x3007: return REVERB_RELEASE;
            case 0x3008: return REVERB_HIGHCUT;
            case 0x3009: return REVERB_TIME;
            case 0x300A: return REVERB_HIGHDAMP;
            case 0x300B: return REVERB_SMOOTH;
            case 0x300C: return REVERB_VOLUME;
            case 0x300D: return REVERB_WIDTH;

            case 0x3014: return ECHO;
            case 0x3015: return ECHO_TYPE;
            case 0x3016: return ECHO_DELAY;
            case 0x3017: return ECHO_FEEDBACK;
            case 0x3018: return ECHO_HIGHCUT;
            case 0x3019: return ECHO_VOLUME;
            case 0x301A: return ECHO_WIDTH;

            case 0x3050: return CTLROOM_MAINOUT;
            case 0x3051: return CTLROOM_MAINMONO;
            case 0x3052: return CTLROOM_MUTEENABLE;
            case 0x3053: return CTLROOM_DIMREDUCTION;
            case 0x3054: return CTLROOM_DIM;
            case 0x3055: return CTLROOM_RECALLVOLUME;

            case 0x3064: return CLOCK_SOURCE;
            case 0x3065: return CLOCK_SAMPLERATE;
            case 0x3066: return CLOCK_WCKSINGLE;
            case 0x3067: return CLOCK_WCKTERM;

            // Kind of lost the thread on the HARDWARE things... maybe playing with it will make it more obvious?
            // Not obviously existing:
            // case 0x3078: AES_INPUT;  // 0=XLR, 1=Optical 2
            // case 0x307A: AES_CHANNEL_STATUS;  // 0=Consumer, 1=Pro
            // case 0x307D: HARDWARE_STANDALONEMIDI; // ? next to this, and there's another one below?
            case 0x3200: return HARDWARE_DSPVERLOAD;
            case 0x3201: return HARDWARE_DSPAVAIL;
            case 0x3202: return HARDWARE_DSPSTATUS;
            case 0x3203: return HARDWARE_ARCDELTA;

			// TODO: This is a guess, need to be confirmed
			case 0x33FD: return REFRESH; // Indicator for refresh done

			// Durec status [r]
			case 0x3580: return DUREC_STATUS;
			case 0x3581: return DUREC_TIME;
			case 0x3582: return UNKNOWN;
			case 0x3583: return DUREC_USBLOAD;
			case 0x3584: return DUREC_TOTALSPACE;
			case 0x3585: return DUREC_FREESPACE;
			case 0x3586: return DUREC_NUMFILES;
			case 0x3587: return DUREC_FILE;
			case 0x3588: return DUREC_NEXT;
			case 0x3589: return DUREC_RECORDTIME;
			case 0x358A: return DUREC_INDEX;
			case 0x358B: return DUREC_NAME0;
			case 0x358C: return DUREC_NAME1;
			case 0x358D: return DUREC_NAME2;
			case 0x358E: return DUREC_NAME3;
			case 0x358F: return DUREC_INFO;
			case 0x3590: return DUREC_LENGTH;

        }
        if (reg >= ROOM_EQ_BASE) {
            p->out = (reg - ROOM_EQ_BASE) / 0x20;
            if (p->out > LEN(outputs))
                return -1;
            switch (reg - (p->out * 0x20)) {
                case 0x00: return ROOMEQ_DELAY;
                case 0x01: return ROOMEQ;
                case 0x02: return ROOMEQ_BAND1TYPE;
                case 0x03: return ROOMEQ_BAND1GAIN;
                case 0x04: return ROOMEQ_BAND1FREQ;
                case 0x05: return ROOMEQ_BAND1Q;
                case 0x06: return ROOMEQ_BAND2GAIN;
                case 0x07: return ROOMEQ_BAND2FREQ;
                case 0x08: return ROOMEQ_BAND2Q;
                case 0x09: return ROOMEQ_BAND3GAIN;
                case 0x0A: return ROOMEQ_BAND3FREQ;
                case 0x0B: return ROOMEQ_BAND3Q;
                case 0x0C: return ROOMEQ_BAND4GAIN;
                case 0x0D: return ROOMEQ_BAND4FREQ;
                case 0x0E: return ROOMEQ_BAND4Q;
                case 0x0F: return ROOMEQ_BAND5GAIN;
                case 0x10: return ROOMEQ_BAND5FREQ;
                case 0x11: return ROOMEQ_BAND5Q;
                case 0x12: return ROOMEQ_BAND6GAIN;
                case 0x13: return ROOMEQ_BAND6FREQ;
                case 0x14: return ROOMEQ_BAND6Q;
                case 0x15: return ROOMEQ_BAND7GAIN;
                case 0x16: return ROOMEQ_BAND7FREQ;
                case 0x17: return ROOMEQ_BAND7Q;
                case 0x18: return ROOMEQ_BAND8TYPE;
                case 0x19: return ROOMEQ_BAND8GAIN;
                case 0x1A: return ROOMEQ_BAND8FREQ;
                case 0x1B: return ROOMEQ_BAND8Q;
                case 0x1C: return ROOMEQ_BAND9TYPE;
                case 0x1D: return ROOMEQ_BAND9GAIN;
                case 0x1E: return ROOMEQ_BAND9FREQ;
                case 0x1F: return ROOMEQ_BAND9Q;
            }
        }
    } else if (reg > 0x4000 && reg < 0x4000 + MIX_REGION_SIZE) {
        p->out = reg >> 6 & 0x3F;
        p->in = reg & 0x3F;
        if (p->out >= LEN(outputs) || p->in >= LEN(inputs))
            return -1;
        return MIX;
    }
    // Not in the wiki so don't know the values:
    //  - AUTOLEVEL_METER
    //  - DYNAMICS_METER
    //  - REFRESH
    //  - DUREC_*
	return -1;
}

static int inputctltoregflag(int const idx, int const reg, int const flags) {
    if (idx > LEN(inputs) || idx < 0)
        return -1;
    if (flags && (inputs[idx].flags & ~flags))
        return -1;
    return idx * N_CHAN_REGS + reg;
}

static int inputctltoreg(int const idx, int const reg) {
    return inputctltoregflag(idx, reg, 0);
}

static int outputctltoregflag(int const idx, int const reg, int const flags) {
    if (idx > LEN(outputs) || idx < 0)
        return -1;
    if (flags && (outputs[idx].flags & ~flags))
        return -1;
    return (idx + LEN(inputs)) * N_CHAN_REGS + reg;
}

static int outputctltoreg(int const idx, int const reg) {
    return outputctltoregflag(idx, reg, 0);
}

static int eqctltoreg(const struct param * const p, int const reg) {
    if (p->in > 0 && p->out > 0)
        return -1;
    int ofs;
    if (p->in > 0)
        ofs = p->in * N_CHAN_REGS + 0x0D;
    else if (p->out > 0)
        ofs = (LEN(inputs) + p->out) * N_CHAN_REGS + 0x0C;
    else
        return -1;
    return reg + ofs;
}

#define N_ROOMEQ_REGS 0x20

static int roomeqreg(int const idx, int const reg) {
  if (idx < 0 || idx >= LEN(outputs))
      return -1;
  return ROOM_EQ_BASE + idx * N_ROOMEQ_REGS + reg;
}

static int ctltoreg(enum control ctl, const struct param *p)
{
	int reg, idx, flags;
	if ((unsigned)p->in < LEN(inputs)) {
		flags = inputs[p->in].flags;
		idx = p->in;
	} else if ((unsigned)p->out < LEN(outputs)) {
		flags = outputs[p->out].flags;
		idx = 94 + p->out;
	}

	switch (ctl) {
		// Inputs
		case INPUT_MUTE:        return inputctltoreg(p->in, 0x00);
		case INPUT_FXSEND:      return inputctltoreg(p->in, 0x01);
		case INPUT_STEREO:      return inputctltoreg(p->in, 0x02);
		case INPUT_RECORD:      return inputctltoreg(p->in, 0x03);
		// 0x04 UNKNOWN
		case INPUT_PLAYCHAN:    return inputctltoreg(p->in, 0x05);
		// case INPUT_WIDTH:       return inputctltoreg(p->in, 0x06);
		case INPUT_MSPROC:      return inputctltoreg(p->in, 0x07);
		case INPUT_PHASE:       return inputctltoreg(p->in, 0x08);
		case INPUT_GAIN:        if (!(flags & INPUT_HAS_GAIN)) break;
								return inputctltoreg(p->in, 0x09);
		case INPUT_REFLEVEL:    if (!(flags & INPUT_HAS_REFLEVEL)) break;
								return inputctltoreg(p->in, 0x0A);
		case INPUT_48V:         if (!(flags & INPUT_HAS_48V)) break;
								return inputctltoreg(p->in, 0x0A);
		case INPUT_HIZ:         if (!(flags & INPUT_HAS_HIZ)) break;
								return inputctltoreg(p->in, 0x0B);
		case INPUT_AUTOSET:		if (!(flags & INPUT_HAS_AUTOSET)) break;
								return inputctltoreg(p->in, 0x0C);
		// Outputs
		case OUTPUT_VOLUME:     return outputctltoreg(p->out, 0x00);
		case OUTPUT_PAN:        return outputctltoreg(p->out, 0x01);
		case OUTPUT_MUTE:       return outputctltoreg(p->out, 0x02);
		case OUTPUT_FXRETURN:   return outputctltoreg(p->out, 0x03);
		case OUTPUT_STEREO:     return outputctltoreg(p->out, 0x04);
		case OUTPUT_RECORD:     return outputctltoreg(p->out, 0x05);
		case OUTPUT_PLAYCHAN:   return outputctltoreg(p->out, 0x06);
		case OUTPUT_PHASE:      return outputctltoreg(p->out, 0x07);
		case OUTPUT_REFLEVEL:   return outputctltoreg(p->out, 0x08);
		case OUTPUT_CROSSFEED:  return outputctltoreg(p->out, 0x09);
			// 0x0A UNKNOWN
		case OUTPUT_VOLUMECAL:  return outputctltoreg(p->out, 0x0B);

			// THE EQ/DYN stuff has different offsets depending on whether we're input or output... so how can we tell?
			// If we're outputting then ->in will be -1 and vice versa... I think.
		case LOWCUT:            return eqctltoreg(p, 0x00);
		case LOWCUT_FREQ:       return eqctltoreg(p, 0x01);
		case LOWCUT_SLOPE:      return eqctltoreg(p, 0x02);
		case EQ:                return eqctltoreg(p, 0x03);
		case EQ_BAND1TYPE:      return eqctltoreg(p, 0x04);
		case EQ_BAND1GAIN:      return eqctltoreg(p, 0x05);
		case EQ_BAND1FREQ:      return eqctltoreg(p, 0x06);
		case EQ_BAND1Q:         return eqctltoreg(p, 0x07);
		case EQ_BAND2GAIN:      return eqctltoreg(p, 0x08);
		case EQ_BAND2FREQ:      return eqctltoreg(p, 0x09);
		case EQ_BAND2Q:         return eqctltoreg(p, 0x0A);
		case EQ_BAND3TYPE:      return eqctltoreg(p, 0x0B);
		case EQ_BAND3GAIN:      return eqctltoreg(p, 0x0C);
		case EQ_BAND3FREQ:      return eqctltoreg(p, 0x0D);
		case EQ_BAND3Q:         return eqctltoreg(p, 0x0E);
		case DYNAMICS:          return eqctltoreg(p, 0x0F);
		case DYNAMICS_GAIN:     return eqctltoreg(p, 0x10);
		case DYNAMICS_ATTACK:   return eqctltoreg(p, 0x11);
		case DYNAMICS_RELEASE:  return eqctltoreg(p, 0x12);
		case DYNAMICS_COMPTHRES:return eqctltoreg(p, 0x13);
		case DYNAMICS_COMPRATIO:return eqctltoreg(p, 0x14);
		case DYNAMICS_EXPTHRES: return eqctltoreg(p, 0x15);
		case DYNAMICS_EXPRATIO: return eqctltoreg(p, 0x16);
		case AUTOLEVEL:         return eqctltoreg(p, 0x17);
		case AUTOLEVEL_MAXGAIN: return eqctltoreg(p, 0x18);
		case AUTOLEVEL_HEADROOM:return eqctltoreg(p, 0x19);
		case AUTOLEVEL_RISETIME:return eqctltoreg(p, 0x1A);

		case REVERB:            return 0x3000;
		case REVERB_TYPE:       return 0x3001;
		case REVERB_PREDELAY:   return 0x3002;
		case REVERB_LOWCUT:     return 0x3003;
		case REVERB_ROOMSCALE:  return 0x3004;
		case REVERB_ATTACK:     return 0x3005;
		case REVERB_HOLD:       return 0x3006;
		case REVERB_RELEASE:    return 0x3007;
		case REVERB_HIGHCUT:    return 0x3008;
		case REVERB_TIME:       return 0x3009;
		case REVERB_HIGHDAMP:   return 0x300A;
		case REVERB_SMOOTH:     return 0x300B;
		case REVERB_VOLUME:     return 0x300C;
		case REVERB_WIDTH:      return 0x300D;
		case ECHO:              return 0x3014;
		case ECHO_TYPE:         return 0x3015;
		case ECHO_DELAY:        return 0x3016;
		case ECHO_FEEDBACK:     return 0x3017;
		case ECHO_HIGHCUT:      return 0x3018;
		case ECHO_VOLUME:       return 0x3019;
		case ECHO_WIDTH:        return 0x301A;

		case MIX:
			if (p->in < 0 || p->in >= LEN(inputs) || p->out < 0 || p->out >= LEN(outputs))
				return -1;
			return 0x4000 | p->out << 6 | p->in;
			// How do playbacks work? They'd be the next chunk.
			// MIX_LEVEL not on wiki

		case ROOMEQ_DELAY:       return roomeqreg(p->out, 0x00);
		case ROOMEQ:             return roomeqreg(p->out, 0x01);
		case ROOMEQ_BAND1TYPE:   return roomeqreg(p->out, 0x02);
		case ROOMEQ_BAND1GAIN:   return roomeqreg(p->out, 0x03);
		case ROOMEQ_BAND1FREQ:   return roomeqreg(p->out, 0x04);
		case ROOMEQ_BAND1Q:      return roomeqreg(p->out, 0x05);
		case ROOMEQ_BAND2GAIN:   return roomeqreg(p->out, 0x06);
		case ROOMEQ_BAND2FREQ:   return roomeqreg(p->out, 0x07);
		case ROOMEQ_BAND2Q:      return roomeqreg(p->out, 0x08);
		case ROOMEQ_BAND3GAIN:   return roomeqreg(p->out, 0x09);
		case ROOMEQ_BAND3FREQ:   return roomeqreg(p->out, 0x0A);
		case ROOMEQ_BAND3Q:      return roomeqreg(p->out, 0x0B);
		case ROOMEQ_BAND4GAIN:   return roomeqreg(p->out, 0x0C);
		case ROOMEQ_BAND4FREQ:   return roomeqreg(p->out, 0x0D);
		case ROOMEQ_BAND4Q:      return roomeqreg(p->out, 0x0E);
		case ROOMEQ_BAND5GAIN:   return roomeqreg(p->out, 0x0F);
		case ROOMEQ_BAND5FREQ:   return roomeqreg(p->out, 0x10);
		case ROOMEQ_BAND5Q:      return roomeqreg(p->out, 0x11);
		case ROOMEQ_BAND6GAIN:   return roomeqreg(p->out, 0x12);
		case ROOMEQ_BAND6FREQ:   return roomeqreg(p->out, 0x13);
		case ROOMEQ_BAND6Q:      return roomeqreg(p->out, 0x14);
		case ROOMEQ_BAND7GAIN:   return roomeqreg(p->out, 0x15);
		case ROOMEQ_BAND7FREQ:   return roomeqreg(p->out, 0x16);
		case ROOMEQ_BAND7Q:      return roomeqreg(p->out, 0x17);
		case ROOMEQ_BAND8TYPE:   return roomeqreg(p->out, 0x18);
		case ROOMEQ_BAND8GAIN:   return roomeqreg(p->out, 0x19);
		case ROOMEQ_BAND8FREQ:   return roomeqreg(p->out, 0x1A);
		case ROOMEQ_BAND8Q:      return roomeqreg(p->out, 0x1B);
		case ROOMEQ_BAND9TYPE:   return roomeqreg(p->out, 0x1C);
		case ROOMEQ_BAND9GAIN:   return roomeqreg(p->out, 0x1D);
		case ROOMEQ_BAND9FREQ:   return roomeqreg(p->out, 0x1E);
		case ROOMEQ_BAND9Q:      return roomeqreg(p->out, 0x1F);

		case HARDWARE_DSPVERLOAD:return 0x3200;
		case HARDWARE_DSPAVAIL:  return 0x3201;
		case HARDWARE_DSPSTATUS: return 0x3202;
		case HARDWARE_ARCDELTA:  return 0x3203;

		case REFRESH:            return 0x3E03; // Reg for dump

		case DUREC_CONTROL:           return 0x3E9A;
		case DUREC_DELETE:            return 0x3E9B;
		case DUREC_FILE:              return 0x3E9C;
		case DUREC_SEEK:              return 0x3E9D;
		case DUREC_PLAYMODE:          return 0x3EA0;
		default: break;

	}
	return -1;
}

const struct device ffufxiii = {
	.id = "ffufxiii",
	.name = "Fireface UFX III",
	.version = 0,
	.flags = DEVICE_HAS_DUREC | DEVICE_HAS_ROOMEQ,
	.inputs = inputs,
	.inputslen = LEN(inputs),
	.outputs = outputs,
	.outputslen = LEN(outputs),
	//.refresh = 0x67CD,
	.refresh = 0x234A,
	.regtoctl = regtoctl,
	.ctltoreg = ctltoreg,
};
