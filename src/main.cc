#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using edge = std::pair<int, int>;

std::vector<int> solve(size_t memb_count, size_t first_member, std::vector<edge> const & edges)
{
	// Граф: списки смежности
	std::vector<std::vector<int>> adj_list(memb_count + 1);
	for (auto const & [u, v] : edges) {
		assert(u >= 0);
		adj_list[(size_t)u].push_back(v);
	}

	std::vector<int> time(memb_count + 1, INT_MAX); // Время, когда член узнал анекдот
	std::vector<int> balance(memb_count + 1, 0);	// Баланс (премии и штрафы)
	time[first_member] = 0;

	// Очередь: (время, номер вершины). Меньшие номера обрабатываются первыми
	std::priority_queue<edge, std::vector<edge>, std::greater<>> Q;
	Q.push({0, first_member});

	while (!Q.empty()) {
		// Извлекаем вершины текущего заседания
		int current_time = Q.top().first;
		std::vector<int> current_vertices;
		while (!Q.empty() && Q.top().first == current_time) {
			auto [t, u] = Q.top();
			Q.pop();
			current_vertices.push_back(u);
		}

		// Обрабатываем вершины в порядке возрастания номеров
		std::sort(current_vertices.begin(), current_vertices.end());
		for (int u : current_vertices) {
			assert(u >= 0);
			for (int v : adj_list[(size_t)u]) {
				assert(v >= 0);
				bool new_anec = time[(size_t)v] == INT_MAX;
				if (new_anec) {
					// Новый анекдот
					time[(size_t)v] = time[(size_t)u] + 1;
					balance[(size_t)u] += 1000;
					Q.push({time[(size_t)v], v});
				} else {
					// Известный анекдот
					balance[(size_t)u] -= 1000;
				}
			}
		}
	}

	// Возвращаем баланс для вершин 1..N
	std::vector<int> result(memb_count);
	for (size_t i = 1; i <= memb_count; ++i) {
		result[i - 1] = balance[i];
	}
	return result;
}

int main()
{
	size_t memb_count, first_member;
	std::cout << "Число участников: ";
	std::cin >> memb_count;
	if (!std::cin)
		return -1;
	std::cout << "Первый участник: ";
	std::cin >> first_member;
	if (!std::cin)
		return -1;

	size_t edge_count;
	std::cout << "Число пар: ";
	std::cin >> edge_count;
	if (!std::cin)
		return -1;

	std::vector<edge> edges(edge_count);
	for (int i{}; auto & [a, b] : edges) {
		std::cout << ++i << "-я пара: ";
		std::cin >> a >> b;
		if (!std::cin)
			return -1;
	}

	std::vector<int> balance_of_everyone = solve(memb_count, first_member, edges);
	for (int i{}; int x : balance_of_everyone) {
		std::cout << "Баланс " << ++i << "-го: " << x << '\n';
	}

	return 0;
}
