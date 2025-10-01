#include <elf_writer.h>
#include <elfio/elfio.hpp>
#include <sstream>
#include <vector>

namespace lada_binary {

auto elf_writer::set_machine_code(std::span<uint8_t const> const& machineCode) -> void {
    _machineCode = machineCode;
}

auto elf_writer::write(std::string const& filename) -> std::vector<uint8_t> {
    constexpr ELFIO::Elf64_Addr BASE_ADDR = 0x400000;
    constexpr ELFIO::Elf64_Addr CODE_ADDR = BASE_ADDR + 0x1000;
    constexpr ELFIO::Elf_Xword PAGE_SIZE = 0x1000;

    ELFIO::elfio writer;
    writer.create(ELFIO::ELFCLASS64, ELFIO::ELFDATA2LSB);
    writer.set_type(ELFIO::ET_EXEC);
    writer.set_machine(ELFIO::EM_X86_64);
    writer.set_os_abi(ELFIO::ELFOSABI_LINUX);

    // --- .text section ---
    auto* text_sec = writer.sections.add(".text");
    text_sec->set_type(ELFIO::SHT_PROGBITS);
    text_sec->set_flags(ELFIO::SHF_ALLOC | ELFIO::SHF_EXECINSTR);
    text_sec->set_addr_align(16);
    text_sec->set_address(CODE_ADDR);
    text_sec->set_data(reinterpret_cast<const char*>(_machineCode.data()), _machineCode.size());

    // --- program segment ---
    auto* text_seg = writer.segments.add();
    text_seg->set_type(ELFIO::PT_LOAD);
    text_seg->set_virtual_address(CODE_ADDR);
    text_seg->set_physical_address(CODE_ADDR);
    text_seg->set_file_size(_machineCode.size());
    text_seg->set_memory_size(_machineCode.size());
    text_seg->set_flags(ELFIO::PF_X | ELFIO::PF_R);
    text_seg->set_align(PAGE_SIZE);
    text_seg->add_section_index(text_sec->get_index(), text_sec->get_addr_align());

    writer.set_entry(CODE_ADDR);

    std::ostringstream oss(std::ios::binary);
    writer.save(oss);
    auto data = oss.str();

    if (!filename.empty()) {
        writer.save(filename);
    }

    return std::vector<uint8_t>(data.begin(), data.end());
}

} // namespace lada_binary
