#ifndef JSONANIMIO_H__
#define JSONANIMIO_H__
/*
    JsonAnimIO.h / .cpp
    -------------------
    Simple helper for exporting an AnimMgr (and the AnimObjects that own one)
    to a human‑readable JSON file, and recreating the same data back again.

    * Depends on: nlohmann::json (single‑header) – just add the header to the
      include path (`#include <nlohmann/json.hpp>`).
    * Does **not** attempt to serialise runtime‑only values such as the current
      animation index or timers – only the *static* description that normally
      lives in your .anim text files.
    * Uses the LUTs created in Animation.h to convert enum values
      ↔ strings, so the JSON stays friendly.

    Typical usage
    -------------
        AnimObject player;
        // …build animations by hand or load the legacy text format…
        JsonAnimIO::saveToFile("Config/Animations/player.json", player.animMgr);

        AnimObject enemy;
        JsonAnimIO::loadFromFile("Config/Animations/player.json", enemy.animMgr);

*/

#include <nlohmann/json.hpp>
#include <fstream>
#include <handlers/AnimMgr.h>
#include <SFML/Graphics/Rect.hpp>
using json = nlohmann::json;

namespace JsonAnimIO
{
    class JSonAnimIO
    {
    public:

        // ───────────────────────────────────────────────────────── helpers ────
        static json vecToJson(const sf::Vector2f& v)
        {
            return { v.x, v.y };
        }

        static sf::Vector2f jsonToVec(const nlohmann::json& j)
        {
            return { j.at(0).get<float>(), j.at(1).get<float>() };
        }

        static json rectToJson(const sf::IntRect& r)
        {
            int left = r.position.x;
            int top = r.position.y;
            int width = r.size.x;
            int height = r.size.y;


            return { left, top, width, height };
        }

        static sf::IntRect jsonToRect(const json& j)
        {
            sf::IntRect r;
            r = { {j.at(0).get<int>(), j.at(1).get<int>()},{j.at(2).get<int>(),j.at(3).get<int>()} };
            return r;
        }


        // ───────────────────────────────────────── enum <‑> string helpers ────
        static std::string toString(AnimDir d) { return FSMStateNameLUT.at(static_cast<AnimName>(d)); }
        static AnimDir      toDir(const std::string& s) { return DirectionLUT.at(s); }

        static std::string toString(AnimName n) { return FSMStateNameLUT.at(n); }
        static AnimName     toAnim(const std::string& s) { return FSMAnimNameLUT.at(s); }

        static std::string toString(Cfg::Textures id) { return TexStringLUT.at(id); }
        static Cfg::Textures toTex(const std::string& s) { return TextureIDLUT.at(s); }

        // ───────────────────────────────────────────────────── core routines ─
        static json serialiseAnimation(const Animation& anim)
        {

            json j;
            j["name"] = toString(anim.name);
            j["texID"] = toString(anim.texID);
            j["frameDelay"] = anim.frameDelay;
            j["loopWaitDelay"] = anim.loopWaitDelay;
            j["loops"] = anim.loops;
            j["loopWaits"] = anim.loopWaits;
            j["numFrames"] = anim.numFrames;

            // directions
            j["frames"]["left"] = serialiseFrames(anim.leftFrames);
            j["frames"]["right"] = serialiseFrames(anim.rightFrames);
            j["frames"]["down"] = serialiseFrames(anim.downFrames);
            j["frames"]["up"] = serialiseFrames(anim.upFrames);
            j["frames"]["upLeft"] = serialiseFrames(anim.upLeftFrames);
            j["frames"]["upRight"] = serialiseFrames(anim.upRightFrames);
            j["frames"]["downLeft"] = serialiseFrames(anim.downLeftFrames);
            j["frames"]["downRight"] = serialiseFrames(anim.downRightFrames);

            // offsets and sizes (optional – omit if empty)
#define COPY_CAT(CAT, MEMBER)                                                        \
        if (!anim.MEMBER.empty()) j[CAT][#MEMBER] = serialiseVecs(anim.MEMBER);

            COPY_CAT("offsets", leftOffsets)  COPY_CAT("offsets", rightOffsets)
                COPY_CAT("offsets", downOffsets)  COPY_CAT("offsets", upOffsets)
                COPY_CAT("offsets", upLeftOffsets) COPY_CAT("offsets", upRightOffsets)
                COPY_CAT("offsets", downLeftOffsets) COPY_CAT("offsets", downRightOffsets)

                COPY_CAT("sizes", leftSizes)  COPY_CAT("sizes", rightSizes)
                COPY_CAT("sizes", downSizes)  COPY_CAT("sizes", upSizes)
                COPY_CAT("sizes", upLeftSizes) COPY_CAT("sizes", upRightSizes)
                COPY_CAT("sizes", downLeftSizes) COPY_CAT("sizes", downRightSizes)

                COPY_CAT("bullets", leftBulletAnchors)  COPY_CAT("bullets", rightBulletAnchors)
                COPY_CAT("bullets", downBulletAnchors)  COPY_CAT("bullets", upBulletAnchors)
                COPY_CAT("bullets", upLeftBulletAnchors) COPY_CAT("bullets", upRightBulletAnchors)
                COPY_CAT("bullets", downLeftBulletAnchors) COPY_CAT("bullets", downRightBulletAnchors)
#undef COPY_CAT

                return j;

        }
        static Animation      deserialiseAnimation(const json& j)
        {

            Animation a;
            a.name = toAnim(j.at("name").get<std::string>());
            a.texID = toTex(j.at("texID").get<std::string>());
            a.numFrames = j.at("numFrames").get<int>();
            a.frameDelay = j.value("frameDelay", 0.f);
            a.loopWaitDelay = j.value("loopWaitDelay", 0.f);
            a.loops = j.value("loops", true);
            a.loopWaits = j.value("loopWaits", false);

            const auto& frames = j.at("frames");
#define DIR_LOAD(KEY, VEC) if(frames.contains(#KEY)) deserialiseFrames(frames.at(#KEY), a.VEC);
            DIR_LOAD(left, leftFrames)
                DIR_LOAD(right, rightFrames)
                DIR_LOAD(down, downFrames)
                DIR_LOAD(up, upFrames)
                DIR_LOAD(upLeft, upLeftFrames)
                DIR_LOAD(upRight, upRightFrames)
                DIR_LOAD(downLeft, downLeftFrames)
                DIR_LOAD(downRight, downRightFrames)
#undef DIR_LOAD

                auto loadOptionalVecs = [&](const char* category) {
                if (j.contains(category)) {
                    const auto& cat = j.at(category);
#define DIR_LOADV(KEY, OFF, VEC) if(cat.contains(#KEY)) deserialiseVecs(cat.at(#KEY), a.VEC);
                    DIR_LOADV(left, leftOffsets, leftOffsets)
                        DIR_LOADV(right, rightOffsets, rightOffsets)
                        DIR_LOADV(down, downOffsets, downOffsets)
                        DIR_LOADV(up, upOffsets, upOffsets)
                        DIR_LOADV(upLeft, upLeftOffsets, upLeftOffsets)
                        DIR_LOADV(upRight, upRightOffsets, upRightOffsets)
                        DIR_LOADV(downLeft, downLeftOffsets, downLeftOffsets)
                        DIR_LOADV(downRight, downRightOffsets, downRightOffsets)
#undef DIR_LOADV
                }
                };

            loadOptionalVecs("offsets");
            loadOptionalVecs("sizes");
            loadOptionalVecs("bullets");

            return a;
        }

        static json serialiseMgr(const AnimMgr& mgr)
        {
            json j;
            j["animations"] = json::array();

            for (auto animName : { AnimName::Idle,  AnimName::Running,AnimName::Shooting, AnimName::RunningAndShooting, AnimName::Invariant })
            {
                const Animation* anim = mgr.getAnimationIfExists(animName); // you will need to expose this helper in AnimMgr
                if (anim) j["animations"].push_back(serialiseAnimation(*anim));
            }
            return j;
        }
        static void           deserialiseMgr(const json& j, AnimMgr& mgr)
        {
            for (const auto& a : j.at("animations"))
            {
                Animation anim = deserialiseAnimation(a);
                mgr.setAnimation(anim.name, std::move(anim)); // likewise, helper required
            }
        }

        // Convenience wrappers that talk directly to files / strings
        static bool saveToFile(const std::string& path, const AnimMgr& mgr)
        {
            std::ofstream out(path);
            if (!out.is_open()) return false;
            out << serialiseMgr(mgr).dump(4);
            return true;
        }
        static bool loadFromFile(const std::string& path, AnimMgr& mgr)
        {
            std::ifstream in(path);
            if (!in.is_open()) return false;
            json j; in >> j;
            deserialiseMgr(j, mgr);
            return true;
        }
        static std::string toString(const AnimMgr& mgr)
        {
            return serialiseMgr(mgr).dump();
        }
        static bool fromString(const std::string& data, AnimMgr& mgr)
        {
            auto j = nlohmann::json::parse(data, nullptr, false);
            if (j.is_discarded()) return false;
            deserialiseMgr(j, mgr);
            return true;
        }

        static json serialiseFrames(const std::vector<sf::IntRect>& frames)
        {
            json arr = json::array();
            for (const auto& r : frames) arr.push_back(rectToJson(r));
            return arr;
        }

        static json serialiseVecs(const std::vector<sf::Vector2f>& v)
        {
            json arr = nlohmann::json::array();
            for (const auto& p : v) arr.push_back(vecToJson(p));
            return arr;
        }


        static void deserialiseFrames(const json& j, std::vector<sf::IntRect>& out)
        {
            out.reserve(j.size());
            for (const auto& e : j) out.emplace_back(jsonToRect(e));
        }

        static void deserialiseVecs(const json& j, std::vector<sf::Vector2f>& out)
        {
            out.reserve(j.size());
            for (const auto& e : j) out.emplace_back(jsonToVec(e));
        }

    };
}

#endif // JSONANIMIO_H__