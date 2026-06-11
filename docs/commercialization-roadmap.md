# Commercialization Roadmap

## Where We Are Now

Both products exist as validated prototypes:

- **Lumos AI Glasses** is fully built and functional. It has been demonstrated publicly and won India's largest GenAI student challenge at the OpenAI Academy × NxtWave Buildathon Maharashtra (India AI Impact Summit 2026, prize: ₹5,00,000).
- **ASL Gloves** has completed circuit design, PCB design, firmware, sign-to-speech, speech-to-sign, and the mobile application. Final wearable assembly and integration testing are pending (Round 2).

The core technology is proven. The path from here to a real product involves three phases.

## Phase 1 — Hardening (Current → 6 Months)

The goal of this phase is to move from "demo that works" to "prototype that someone can use daily."

- Complete ASL Gloves wearable assembly and integration testing
- Run structured user testing sessions with individuals from the target disability communities
- Collect failure modes, edge cases, and comfort feedback
- Improve gesture recognition accuracy and reduce sign-to-speech latency
- Replace cloud AI calls in Lumos with on-device models (target: ONNX runtime on Raspberry Pi, no internet required)
- Write proper calibration tools so users can train the gloves to their personal signing style

## Phase 2 — Product Refinement (6–18 Months)

The goal of this phase is to produce something manufacturable and certifiable.

- Miniaturise Lumos hardware into a purpose-built frame (current version uses off-the-shelf components in a CAD-designed housing)
- Redesign ASL Gloves PCB for production — reduce size, improve durability, waterproofing
- Add emergency SOS integration: a specific gesture triggers an alert to a registered contact with location
- Add haptic notifications to complement tactile finger movements
- Expand sign language support beyond ASL — ISL (Indian Sign Language) is the immediate priority
- Begin regulatory review for medical/assistive device classification in India and target export markets

## Phase 3 — Market Entry (18–36 Months)

- Partner with disability organisations, rehabilitation hospitals, and special education schools for pilot deployments
- Explore government procurement channels — India's ADIP scheme (Assistance to Disabled Persons) funds assistive device distribution
- Pursue NGO and CSR partnerships for subsidised distribution in lower-income communities
- Launch a commercial product line alongside an open-source community edition
- Explore international markets: Southeast Asia, Africa, and other regions where deafblind support infrastructure is limited and cost sensitivity is high

## Revenue Model Considerations

| Model | Notes |
|---|---|
| Direct consumer sales | High margin but limited reach at current price points |
| Institutional / hospital sales | Larger contracts, longer cycles, strong validation signal |
| Government procurement | High volume, requires certification and local manufacturing |
| Open core | Hardware open-source, value-added software/services paid |
| Licensing | License the gesture recognition and tactile feedback IP to larger assistive tech companies |

## What the Buildathon Win Unlocks

The ₹5,00,000 prize and associated recognition at India AI Impact Summit 2026 provides:

- Credibility for institutional partnerships and grant applications
- Media coverage that helps reach the disability community directly
- Access to the NxtWave and OpenAI Academy mentor and investor networks
- A validated proof point for any future funding conversations

This is the starting point, not the finish line.