// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * libfdt - Flat Device Tree manipulation
 * Copyright (C) 2014 David Gibson <david@gibson.dropbear.id.au>
 * Copyright (C) 2018 embedded brains GmbH
 */
#include "libfdt_env.h"

#include "fdt.h"
#include "libfdt.h"

#include "libfdt_internal.h"

static int fdt_cells(const void *fdt, int nodeoffset, const char *name)
{
	const fdt32_t *c;
	uint32_t val;
	int len;

	c = fdt_getprop(fdt, nodeoffset, name, &len);
	if (!c)
		return len;

	if (len != sizeof(*c))
		return -FDT_ERR_BADNCELLS;

	val = fdt32_to_cpu(*c);
	if (val > FDT_MAX_NCELLS)
		return -FDT_ERR_BADNCELLS;

	return (int)val;
}

int fdt_address_cells(const void *fdt, int nodeoffset)
{
	int val;

	val = fdt_cells(fdt, nodeoffset, "#address-cells");
	if (val == 0)
		return -FDT_ERR_BADNCELLS;
	if (val == -FDT_ERR_NOTFOUND)
		return 2;
	return val;
}

int fdt_size_cells(const void *fdt, int nodeoffset)
{
	int val;

	val = fdt_cells(fdt, nodeoffset, "#size-cells");
	if (val == -FDT_ERR_NOTFOUND)
		return 1;
	return val;
}

/* This function assumes that [address|size]_cells is 1 or 2 */
int fdt_appendprop_addrrange(void *fdt, int parent, int nodeoffset,
			     const char *name, uint64_t addr, uint64_t size)
{
	int addr_cells, size_cells, ret;
	uint8_t data[sizeof(fdt64_t) * 2], *prop;

	ret = fdt_address_cells(fdt, parent);
	if (ret < 0)
		return ret;
	addr_cells = ret;

	ret = fdt_size_cells(fdt, parent);
	if (ret < 0)
		return ret;
	size_cells = ret;

	/* check validity of address */
	prop = data;
	if (addr_cells == 1) {
		if ((addr > UINT32_MAX) || (((uint64_t) UINT32_MAX + 1 - addr) < size))
			return -FDT_ERR_BADVALUE;

		fdt32_st(prop, (uint32_t)addr);
	} else if (addr_cells == 2) {
		fdt64_st(prop, addr);
	} else {
		return -FDT_ERR_BADNCELLS;
	}

	/* check validity of size */
	prop += addr_cells * sizeof(fdt32_t);
	if (size_cells == 1) {
		if (size > UINT32_MAX)
			return -FDT_ERR_BADVALUE;

		fdt32_st(prop, (uint32_t)size);
	} else if (size_cells == 2) {
		fdt64_st(prop, size);
	} else {
		return -FDT_ERR_BADNCELLS;
	}

	return fdt_appendprop(fdt, nodeoffset, name, data,
			      (addr_cells + size_cells) * sizeof(fdt32_t));
}

/**
 * FUNCIONALIDAD ADICIONAL AÑADIDA PARA LECTURA DE LOS REGISTROS
 *
*/

static int _fdt_read_cells(const fdt32_t *cells, unsigned int n, uint64_t *value)
{
    unsigned int i;

    if (n > 2)
        return -FDT_ERR_BADNCELLS;

    *value = 0;
    for (i = 0; i < n; i++) {
        *value <<= (sizeof(*cells) * 8);
        *value |= fdt32_to_cpu(cells[i]);
    }

    return 0;
}

int fdt_simple_addr_size(const void *fdt, int nodeoffset, int idx, uint64_t *addrp, uint64_t *sizep)
{
    int parent;
    int ac, sc, reg_stride;
    int res;
    const fdt32_t *reg;

    reg = fdt_getprop(fdt, nodeoffset, "reg", &res);
    if (res < 0)
        return res;

    parent = fdt_parent_offset(fdt, nodeoffset);
    if (parent == -FDT_ERR_NOTFOUND) /* an node without a parent does not have _any_ number of cells */
        return -FDT_ERR_BADNCELLS;
    if (parent < 0)
        return parent;

    ac = fdt_address_cells(fdt, parent);
    if (ac < 0)
        return ac;

    sc = fdt_size_cells(fdt, parent);
    if (sc < 0)
        return sc;

    reg_stride = ac + sc;

    /* res is the number of bytes read and must be an even multiple of the sum of address cells and size cells */
    if ((res % (reg_stride * sizeof(*reg))) != 0)
        return -FDT_ERR_BADVALUE;

    if (addrp) {
        res = _fdt_read_cells(&reg[reg_stride * idx], ac, addrp);
        if (res < 0)
            return res;
    }
    if (sizep) {
        res = _fdt_read_cells(&reg[ac + reg_stride * idx], sc, sizep);
        if (res < 0)
            return res;
    }

    return 0;
}

