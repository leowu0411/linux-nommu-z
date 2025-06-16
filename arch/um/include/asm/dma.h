/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_DMA_H
#define __UM_DMA_H

#include <asm/io.h>

/**
 * now the PCI core driver depends on CONFIG_MMU in linus tree, nommu
 * UML cannot build with PCI but without PCI kunit doesn't build due
 * to the dependency to the CONFIG_VIRTIO_UML.
 *
 * This is a workaround to silence build failures on kunit, which is
 * valid until nommu UML supports PCI drivers (e.g., virtio-pci) in a
 * future.
 */
#ifndef CONFIG_MMU
#undef PCI_IOBASE
#endif

extern unsigned long uml_physmem;

#define MAX_DMA_ADDRESS (uml_physmem)

#endif
