
#ifndef PACKETS_HPP
#define PACKETS_HPP

namespace MCServer {
namespace Network {

enum Packets {
    PACKET_KEEP_ALIVE = 0x00,
    PACKET_LOGIN_REQUEST = 0x01,
    PACKET_HANDSHAKE = 0x02,
    PACKET_CHAT_MESSAGE = 0x03,
    PACKET_TIME_UPDATE = 0x04,
    PACKET_ENTITY_EQUIPMENT = 0x05,
    PACKET_SPAWN_POSITION = 0x06,
    PACKET_USE_ENTITY = 0x07,
    PACKET_UPDATE_HEALTH = 0x08,
    PACKET_RESPAWN = 0x09,
    PACKET_PLAYER = 0x0A,
    PACKET_PLAYER_POSITION = 0x0B,
    PACKET_PLAYER_LOOK = 0x0C,
    PACKET_PLAYER_POSITION_AND_LOOK = 0x0D,
    PACKET_PLAYER_DIGGING = 0x0E,
    PACKET_PLAYER_BLOCK_PLACEMENT = 0x0F,
    PACKET_HELD_ITEM_CHANGE = 0x10,
    PACKET_USE_BED = 0x11,
    PACKET_ANIMATION = 0x12,
    PACKET_ENTITY_ACTION = 0x13,
    PACKET_SPAWN_NAMED_ENTITY = 0x14,
    PACKET_SPAWN_DROPPED_ITEM = 0x15,
    PACKET_COLLECT_ITEM = 0x16,
    PACKET_SPAWN_OBJECT_OR_VEHICLE = 0x17,
    PACKET_SPAWN_MOB = 0x18,
    PACKET_SPAWN_PAINTING = 0x19,
    PACKET_SPAWN_EXPERIENCE_ORB = 0x1A,
    PACKET_ENTITY_VELOCITY = 0x1C,
    PACKET_DESTROY_ENTITY = 0x1D,
    PACKET_ENTITY = 0x1E,
    PACKET_ENTITY_RELATIVE_MOVE = 0x1F,
    PACKET_ENTITY_LOOK = 0x20,
    PACKET_ENTITY_LOOK_AND_RElATIVE_MOVE = 0x21,
    PACKET_ENTITY_TELEPORT = 0x22,
    PACKET_ENTITY_HEAD_LOOK = 0x23,
    PACKET_ENTITY_STATUS = 0x26,
    PACKET_ATTACH_ENTITY = 0x27,
    PACKET_ENTITY_METADATA = 0x28,
    PACKET_ENTITY_EFFECT = 0x29,
    PACKET_REMOVE_ENTITY_EFFECT = 0x2A,
    PACKET_SET_EXPERIENCE = 0x2B,
    PACKET_CHUNK_DATA = 0x33,
    PACKET_MULTI_BLOCK_CHANGE = 0x34,
    PACKET_BLOCK_CHANGE = 0x35,
    PACKET_BLOCK_ACTION = 0x36,
    PACKET_BLOCK_BREAK_ANIMATION = 0x37,
    PACKET_MAP_CHUNK_BULK = 0x38,
    PACKET_EXPLOSION = 0x3C,
    PACKET_SOUND_OR_PARTICLE_EFFECT = 0x3D,
    PACKET_NAMED_SOUND_EFFECT = 0x3E,
    PACKET_CHANGE_GAME_STATE = 0x46,
    PACKET_THUNDERBOLT = 0x47,
    PACKET_OPEN_WINDOW = 0x64,
    PACKET_CLOSE_WINDOW = 0x65,
    PACKET_CLICK_WINDOW = 0x66,
    PACKET_SET_SLOT = 0x67,
    PACKET_SET_WINDOW_ITEMS = 0x68,
    PACKET_UPDATE_WINDOW_PROPERTY = 0x69,
    PACKET_CONFIRM_TRNSACTION = 0x6A,
    PACKET_CREATIVE_INVENTORY_ACTION = 0x6B,
    PACKET_ENCHANT_ITEM = 0x6C,
    PACKET_UPDATE_SIGN = 0x82,
    PACKET_ITEM_DATA = 0x83,
    PACKET_UPDATE_TILE_ENTITY = 0x84,
    PACKET_INCREMENT_STATISTIC = 0xC8,
    PACKET_PLAYER_LIST_ITEM = 0xC9,
    PACKET_PLAYER_ABILITIES = 0xCA,
    PACKET_TAB_COMPLETE = 0xCB,
    PACKET_LOCALE_AND_VIEW_DISTANCE = 0xCC,
    PACKET_CLIENT_STATUSES = 0xCD,
    PACKET_PLUGIN_MESSAGE = 0xFA,
    PACKET_ENCRYPTION_KEY_RESPONSE = 0xFC,
    PACKET_ENCRYPTION_KEY_REQUEST = 0xFD,
    PACKET_SERVER_LIST_PING = 0xFE,
    PACKET_DISCONNECT_KICK = 0xFF,
};

}
}

#endif
