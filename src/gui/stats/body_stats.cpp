#include "body_stats.h"
#include "../../components/body/body.h"
#include <sstream>
#include <utility>


void gui::Body_stats::redraw()
{
    stats.draw();
}

void gui::Body_stats::activateEvent()
{
    stats.activate();
}

void gui::Body_stats::deactivateEvent()
{
    stats.deactivate();
}

std::string gui::Body_stats::generateText()
{
    using enum Body_attribute;
    using enum Body_stat;
    using enum Body_part_attribute;
    using enum Body_part_stat;

    std::stringstream sstr;

    if (const auto body = registry.try_get<Body>(entity))
    {
        bool body_empty = true;
        const auto insert = [&](auto&& f)
        {
            if (!std::exchange(body_empty, false)) sstr << '\n';
            f();
        };

        if (const auto hp = body->getPartsStat(HP))
        {
            insert([&]{ sstr << "HP: " << *hp << "/" << body->getPartsAttribute(registry, MAX_HP).value_or(-1); });
        }
        if (const auto mp = body->getStat(MANA))
        {
            insert([&] { sstr << "MP: " << *mp << "/" << body->getAttribute(registry, MAX_MANA).value_or(-1); });
        }
        if (const auto mp_regen = body->getAttribute(registry, MANA_REGEN))
        {
            insert([&]{ sstr << "MP REGEN: " << *mp_regen; });
        }
        if (const auto satiety = body->getStat(SATIETY))
        {
            insert([&]{ sstr << "SATIETY: " << *satiety << "/" << body->getAttribute(registry, MAX_SATIETY).value_or(-1); });
        }
        if (const auto strength = body->getAttribute(registry, STRENGTH))
        {
            insert([&]{ sstr << "STR: " << *strength; });
        }
        if (const auto dexterity = body->getAttribute(registry, DEXTERITY))
        {
            insert([&]{ sstr << "DEX: " << *dexterity; });
        }

        if (!body_empty) sstr << "\n\n";

        body->graph.foreach([&](const Body_part& bp) {
            bool part_empty = true;
            const auto insert = [&](auto&& f)
            {
                if (!std::exchange(part_empty, false)) sstr << ' ';
                f();
            };
            sstr << bp.name() << " (";
            if (const auto hp = bp.getStat(HP))
            {
                insert([&] { sstr << "HP: " << *hp << "/" << bp.getAttribute(registry, MAX_HP).value_or(-1); });
            }
            if (const auto hp_regen = bp.getAttribute(registry, HP_REGEN))
            {
                insert([&]{ sstr << "HP REGEN: " << *hp_regen; });
            }
            if (const auto defense = bp.getAttribute(registry, DEFENSE))
            {
                insert([&] { sstr << "DEF: " << *defense; });   
            }

            sstr << ")\n";
        });
    }

    return sstr.str();
}

gui::Body_stats::Body_stats(const entt::registry& registry, const entt::entity entity)
    : registry{ registry }, entity{ entity }
{
    stats.setParent(this);
    stats.setSizeInfo({ .percentage = {1,1} });
    stats.setLineSpacing(-18); 
}

void gui::Body_stats::update()
{
    if(auto str = generateText(); str != stats.getString()) stats.setString(std::move(str));
    stats.update();
}
