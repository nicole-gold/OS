#include "os.h"
#include <assert.h>

#define PTE_SIZE 64U
#define OFFSET_SIZE 12U
#define SYMBOL_SIZE 9U

uint64_t x;

static const uint64_t get_number_of_tree_levels()
{
	return 5; // TODO: NICOLE
}

static uint64_t get_middle_n_bits(uint64_t value, int n, uint64_t start_bit)
{
	uint64_t mask;

	mask = ((1 << n) - 1) << start_bit;
	return value & mask;
}

static uint64_t get_pte_idx(int part, uint64_t vpn)
{
	uint64_t nr_tree_levels = get_number_of_tree_levels();
	uint64_t start_bit;
	assert (part < nr_tree_levels);

	vpn = vpn >> OFFSET_SIZE;
	start_bit = (nr_tree_levels - part) * SYMBOL_SIZE;
	return get_middle_n_bits(vpn, SYMBOL_SIZE, start_bit);
}

static uint64_t get_pa(uint64_t vpn, uint64_t last_pte)
{
	uint64_t vpo = get_middle_n_bits(vpn, OFFSET_SIZE , 0);
	uint64_t ppn =  (last_pte >> 12) << 12;
	return ppn + vpo;
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
	int i;
	const uint64_t nr_tree_levels = get_number_of_tree_levels();
	uint64_t pte;
	void *base;

	base = phys_to_virt(pt);
	for (i = 0; i < nr_tree_levels; i++) {
		pte = (uint64_t)base + (get_pte_idx(i, vpn) * PTE_SIZE);
		if (!get_middle_n_bits(pte, 1 , 0)) {
			return NO_MAPPING;
		}

		base = phys_to_virt(pte);
	}


	return get_pa(vpn, pte);
}
