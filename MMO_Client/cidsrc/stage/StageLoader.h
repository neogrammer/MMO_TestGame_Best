#ifndef STAGELOADER_H__
#define STAGELOADER_H__

#include "StageData.h"
#include <filesystem>

// A tiny, stateless helper namespace that translates Stage JSON ⇆ C++ structs.
// It has **zero** SFML dependencies so the same code can be used by both the
// headless server and the graphical client.
namespace StageLoader {

    /// Parse a UTF‑8 JSON file on disk and return a fully‑populated StageData.
    /// Throws std::runtime_error on any I/O or schema error.
    StageData loadFromJsonFile(const std::filesystem::path& file);

    /// Serialize a StageData back to JSON (pretty‑printed, UTF‑8) for editors.
    void saveToJsonFile(const StageData& stage,
        const std::filesystem::path& file);

} // namespace StageLoader

#endif