# Accessibility Impact

## Who This Is For

### Lumos AI Glasses

Lumos is designed for individuals living with one of three conditions:

- **Visual impairment or blindness** — Lumos provides real-time object detection, scene understanding, OCR-based text reading, and AI-powered contextual narration through audio. Users can navigate environments, read printed text, and get intelligent answers about what is in front of them.

- **Hearing impairment or deafness** — Lumos transcribes speech in real time and displays it on a transparent OLED overlay, enabling participation in conversations without requiring the other person to change how they communicate.

- **Speech impairment or muteness** — Lumos provides text-based communication assistance and AI conversation support, giving users a voice through technology rather than requiring manual typing on a phone.

### ASL Gloves

ASL Gloves are designed specifically for individuals who are blind, deaf, and mute simultaneously — a group that most assistive technologies fail entirely, because every existing solution assumes at least one intact sensory channel.

The gloves work entirely through touch:

- The user communicates outward by signing with their hands. Flex sensors and an IMU capture the gesture, which is processed and converted to speech output for the people around them.
- Others communicate inward by speaking normally. Their speech is transcribed, transmitted via Bluetooth, and replayed as physical finger movements on the user's hand — a tactile alphabet they can feel.

No sight required. No hearing required. No speech required.

## The Gap This Fills

| User profile | Existing solutions | Lumos X ASL Gloves |
|---|---|---|
| Blind only | Screen readers, white canes | Lumos AI Glasses |
| Deaf only | Hearing aids, captioning apps | Lumos AI Glasses |
| Mute only | AAC devices, text-to-speech apps | Lumos AI Glasses |
| Blind + deaf + mute | Almost nothing | ASL Gloves |

The deafblind population is estimated at over 360,000 in India alone. Globally, the number runs into the millions. Most have no independent communication tool.

## How Independence Is Measured

We define impact not by whether the technology works in a controlled demo, but by whether it changes a user's daily dependency on caregivers:

- Can the user initiate a conversation with a stranger without assistance?
- Can the user navigate a new environment without a guide?
- Can the user understand what is being said in a group setting?
- Can the user communicate their own needs without a mediator?

Every feature in both products is evaluated against these questions.

## Affordability and Reach

A product that costs more than a family's monthly income is not an accessibility tool — it is a luxury. The ASL Gloves are built with this constraint as a hard requirement:

- Total bill of materials: **₹2,580**
- Uses commodity hardware available globally (ESP32, SG90 servos, flex sensors)
- Open-source hardware and firmware — anyone can build, modify, or improve it
- Mobile app built with MIT App Inventor — no proprietary development tools required

Lumos currently relies on cloud AI for its most advanced features. The roadmap targets on-device inference to remove the subscription cost dependency entirely.